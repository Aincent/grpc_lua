// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../core/client/impl/client_async_reader_writer_impl.h"

#include "../../../core/client/channel.h"      // for MakeSharedCall()
#include "../../../core/client/impl/cqtag/client_reader_read_cqtag.h"   // for ClientReaderReadCqTag
#include "../../../core/client/impl/cqtag/client_recv_init_md_cqtag.h"  // for ClientRecvInitMdCqTag
#include "../../../core/client/impl/cqtag/client_send_close_cqtag.h"    // for ClientSendCloseCqTag
#include "../../../core/client/impl/cqtag/client_send_init_md_cqtag.h"  // ClientSendInitMdCqTag
#include "../../../core/client/impl/cqtag/client_send_msg_cqtag.h"      // for ClientSendMsgCqTag
#include "../../../core/common/impl/complete_cb.h"          // for CompleteCb

namespace core {

// Todo: SyncGetInitMd();

ClientAsyncReaderWriterImpl::ClientAsyncReaderWriterImpl(
    const ChannelSptr& channel, const std::string& method,
    const CompletionQueueSptr& cq_sptr, int64_t timeout_ms,
    const StatusCb& status_cb)
    : call_sptr_(channel->MakeSharedCall(method, *cq_sptr, timeout_ms)),
      status_cb_(status_cb) {
  assert(cq_sptr);
  assert(call_sptr_);
}  // ClientAsyncReaderWriterImpl()

ClientAsyncReaderWriterImpl::~ClientAsyncReaderWriterImpl() {
  // No CqTag shares this.
  assert(reading_ended_);
  assert(writing_ended_);
}

bool ClientAsyncReaderWriterImpl::Write(const std::string& msg) {
  Guard g(mtx_);
  InitIfNot();
  if (!status_.ok()) {
    assert(reading_ended_ && writing_ended_);
    return false;
  }
  if (writing_closing_ || writing_ended_)
    return false;

  bool is_sending = !msg_queue_.empty();
  msg_queue_.push(msg);
  if (is_sending) return true;
  return SendNext();
}  // Write()

void ClientAsyncReaderWriterImpl::CloseWriting() {
  Guard g(mtx_);
  if (writing_closing_) return;
  writing_closing_ = true;

  if (!status_.ok()) return;
  if (!msg_queue_.empty()) return;  // sending

  // End when all messages are written.
  assert(!has_sent_close_);
  SendClose();
  assert(writing_ended_);
}  // CloseWriting()

void ClientAsyncReaderWriterImpl::ReadEach(const MsgStrCb& msg_cb) {
  Guard g(mtx_);
  if (reading_started_) return;  // already started.
  reading_started_ = true;
  InitIfNot();
  if (!status_.ok()) return;
  msg_cb_ = msg_cb;
  ReadNext();
}  // ReadEach()

void ClientAsyncReaderWriterImpl::SetErrorStatus(const Status& error_status) {
  assert(!error_status.ok());
  Guard g(mtx_);
  if (status_.ok()) {
    EndOnErrorStatus(error_status);
    return;
  }

  // Only replace status, no other actions.
  status_ = error_status;
}  // SetErrorStatus()

void ClientAsyncReaderWriterImpl::OnSent(bool success) {
  Guard g(mtx_);  // Callback need Guard.
  assert(!msg_queue_.empty());
  msg_queue_.pop();  // front msg is sent

  if (!status_.ok()) {
    assert(!status_cb_);  // already called and rest
    return;
  }
  if (!success) {
    SetInternalError("Failed to send message.");
    return;
  }

  if (!msg_queue_.empty()) {
    SendNext();
    return;
  }

  if (!writing_closing_)
    return;
  SendClose();
  assert(writing_ended_);
  if (reading_ended_)
    CallStatusCb();  // Both ended.
}  // OnSent()

void ClientAsyncReaderWriterImpl::OnRead(bool success,
    ClientReaderReadCqTag& tag) {
  Guard g(mtx_);  // Callback needs Guard.
  assert(reading_started_);
  if (!status_.ok()) {
    assert(!status_cb_);  // already called and reset
    return;
  }
  if (!success) {
    SetInternalError("ClientReaderReadCqTag failed.");
    return;
  }
  if (!tag.HasGotMsg()) {  // End of reading.
    reading_ended_ = true;  // Normal end.
    if (writing_ended_)
      CallStatusCb();
    return;
  }

  std::string sMsg;
  Status status = tag.GetResultMsg(sMsg);
  if (!status.ok()) {
    EndOnErrorStatus(status);  // trigger status cb
    return;
  }

  if (msg_cb_) {
    Status status = msg_cb_(sMsg);
    if (!status.ok()) {
      EndOnErrorStatus(status);
      return;
    }
  }

  ReadNext();
}  // OnRead()

void ClientAsyncReaderWriterImpl::InitIfNot() {
  // private function has no Guard
  if (inited_) return;
  inited_ = true;
  if (!status_.ok()) return;

  ClientSendInitMdCqTag* send_tag = new ClientSendInitMdCqTag(call_sptr_);
  if (!send_tag->Start()) {
    delete send_tag;
    SetInternalError("Failed to send init metadata to init bi-directional streaming.");
    return;
  }  // if

  ClientRecvInitMdCqTag* recv_tag = new ClientRecvInitMdCqTag(call_sptr_);
  if (!recv_tag->Start()) {
    delete recv_tag;
    SetInternalError("Failed to receive init metadata to init bi-directional streaming.");
    return;
  }  // if
}  // InitIfNot()

bool ClientAsyncReaderWriterImpl::SendNext() {
  // private function has no Guard
  assert(!msg_queue_.empty());
  assert(status_.ok());
  assert(call_sptr_);

  auto* tag = new ClientSendMsgCqTag(call_sptr_);
  auto sptr = shared_from_this();
  CompleteCb complete_cb = [sptr](bool success) {
    sptr->OnSent(success);
  };
  tag->SetCompleteCb(complete_cb);

  if (tag->Start(msg_queue_.front()))  // Send the front.
    return true;

  delete tag;
  SetInternalError("Failed to write bi-directional streaming.");
  return false;
}  // SendNext()

void ClientAsyncReaderWriterImpl::ReadNext() {
  // private function has no Guard
  assert(status_.ok());
  auto* tag = new ClientReaderReadCqTag(call_sptr_);
  auto sptr = shared_from_this();
  tag->SetCompleteCb([sptr, tag](bool success) {
      sptr->OnRead(success, *tag);
  });
  if (tag->Start()) return;

  delete tag;
  SetInternalError("Failed to async read bi-directional streaming.");
}  // ReadNext()

// Send close to half-close when writing are ended.
void ClientAsyncReaderWriterImpl::SendClose() {
  // private function has no Guard
  assert(writing_closing_);  // Must after CloseWriting().
  assert(!has_sent_close_);
  assert((has_sent_close_ = true));
  assert(status_.ok());

  writing_ended_ = true;  // Normal end.
  ClientSendCloseCqTag* tag = new ClientSendCloseCqTag(call_sptr_);
  if (tag->Start()) return;
  delete tag;
  SetInternalError("Failed to close writing.");  // calls status_cb_
}  // SendClose()

// Set status, call status callback and reset helpers.
void ClientAsyncReaderWriterImpl::SetInternalError(const std::string& sError) {
  // private function has no Guard
  assert(status_.ok());
  EndOnErrorStatus(Status::InternalError(sError));
}

void ClientAsyncReaderWriterImpl::EndOnErrorStatus(
    const Status& error_status) {
  assert(!error_status.ok());
  // private function has no Guard
  assert(status_.ok());
  status_ = error_status;
  CallStatusCb();
  // Set ended on error.
  reading_ended_ = true;
  writing_ended_ = true;
}  // EndOnErrorStatus()

void ClientAsyncReaderWriterImpl::CallStatusCb() {
  // private function has no Guard
  if (!status_cb_) return;
  status_cb_(status_);
  status_cb_ = nullptr;
}

}  // namespace core
