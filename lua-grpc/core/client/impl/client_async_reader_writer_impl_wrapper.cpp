// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../core/client/impl/client_async_reader_writer_impl_wrapper.h"

#include <cassert>

#include "../../../core/client/impl/client_async_reader_writer_impl.h"  // for ClientAsyncReaderWriterImpl

namespace core {

// Todo: SyncGetInitMd();

ClientAsyncReaderWriterImplWrapper::ClientAsyncReaderWriterImplWrapper(
    const ChannelSptr& channel, const std::string& method,
    const CompletionQueueSptr& cq_sptr, int64_t timeout_ms,
    const StatusCb& status_cb)
    : impl_sptr_(new ClientAsyncReaderWriterImpl(channel, method, cq_sptr,
                                                   timeout_ms, status_cb)) {
  assert(cq_sptr);
}

ClientAsyncReaderWriterImplWrapper::~ClientAsyncReaderWriterImplWrapper() {
  impl_sptr_->CloseWriting();  // impl_sptr_ will live on.
}

bool ClientAsyncReaderWriterImplWrapper::Write(const std::string& msg) {
  return impl_sptr_->Write(msg);
}

void ClientAsyncReaderWriterImplWrapper::CloseWriting() {
  impl_sptr_->CloseWriting();
}

void ClientAsyncReaderWriterImplWrapper::ReadEach(const MsgStrCb& msg_cb) {
  impl_sptr_->ReadEach(msg_cb);
}

void ClientAsyncReaderWriterImplWrapper::SetErrorStatus(const Status& error_status) {
  assert(!error_status.ok());
  impl_sptr_->SetErrorStatus(error_status);
}

}  // namespace core
