// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../core/server/impl/server_writer_impl.h"

#include <thread>  // for yield()

#include "../../../core/server/impl/cqtag/server_writer_send_status_cqtag.h"  // for ServerWriterSendStatusCqTag
#include "../../../core/server/impl/cqtag/server_writer_write_cqtag.h"        // for ServerWriterWriteCqTag

namespace core {

ServerWriterImpl::ServerWriterImpl(const CallSptr& call_sptr)
    : call_sptr_(call_sptr) {
  assert(call_sptr);
}

ServerWriterImpl::~ServerWriterImpl() {
  // Because ServerWriterWriteCqTag has a ServerWriterImpl sptr.
  assert(queue_.empty());
  SyncClose(Status::OK);
}

bool ServerWriterImpl::Write(const string& response) {
  bool is_high = false;
  {
    Guard g(mtx_);
    if (queue_.size() >= high_queue_size_)
      is_high = true;
  }

  if (is_high)
    return SyncWrite(response);
  else
    return AsyncWrite(response);
}

bool ServerWriterImpl::SyncWrite(const string& response) {
  if (!AsyncWrite(response))  // Will trigger sending.
    return false;

  while (GetQueueSize())  // need to clear queue on error
    std::this_thread::yield();

  Guard g(mtx_);
  return !closed_;
}

bool ServerWriterImpl::AsyncWrite(const string& response) {
  Guard g(mtx_);

  if (closed_ || close_status_uptr_)
    return false;

  if (is_sending_) {
    queue_.push(response);
    return true;
  }

  assert(queue_.empty());
  return SendMsg(response);
}

size_t ServerWriterImpl::GetHighQueueSize() const {
  Guard g(mtx_);
  return high_queue_size_;
}

void ServerWriterImpl::SetHighQueueSize(size_t high_queue_size) {
  Guard g(mtx_);
  high_queue_size_ = high_queue_size;
}

void ServerWriterImpl::SyncClose(const Status& status) {
  AsyncClose(status);
  while (GetQueueSize())
    std::this_thread::yield();
}

void ServerWriterImpl::AsyncClose(const Status& status) {
  Guard g(mtx_);

  if (closed_ || close_status_uptr_)
    return;

  close_status_uptr_.reset(new Status(status));
  if (is_sending_) return;
  assert(queue_.empty());
  SendStatus();
}

void ServerWriterImpl::TryToWriteNext() {
  Guard g(mtx_);

  if (closed_) return;

  is_sending_ = false;  // Prev msg sending completed.
  if (!queue_.empty()) {
    SendMsg(queue_.front());
    queue_.pop();
    assert(is_sending_);
    return;
  }

  if (close_status_uptr_) {
    SendStatus();
    return;
  }

  // No message and no close.
  assert(!is_sending_);
}

void ServerWriterImpl::SendStatus() {
  assert(!closed_);
  assert(close_status_uptr_);
  assert(queue_.empty());

  closed_ = true;
  const Status& status = *close_status_uptr_;
  using CqTag = ServerWriterSendStatusCqTag;
  CqTag* tag = new CqTag(call_sptr_);
  // Todo: set init md and trail md
  if (tag->StartSend(status, send_init_md_))
    return;
  delete tag;
}

bool ServerWriterImpl::SendMsg(const string& msg) {
  assert(!closed_);
  assert(!is_sending_);
  is_sending_ = true;

  // CqTag shares this and will TryToWriteNext() on cq completion.
  using CqTag = ServerWriterWriteCqTag;
  CqTag* tag = new CqTag(call_sptr_);
  auto sptr = shared_from_this();
  tag->SetCompleteCb([sptr](bool success) {
    // Todo: check success
    sptr->TryToWriteNext();
  });
  if (tag->Start(msg, send_init_md_)) {
    send_init_md_ = false;
    return true;
  }

  delete tag;
  closed_ = true;  // error
  queue_ = std::queue<string>();  // reset to break SyncWrite().
  // Todo: do sth. on error?
  return false;
}

}  // namespace core
