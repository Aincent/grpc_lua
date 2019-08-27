// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../core/client/client_async_writer.h"

#include <cassert>  // for assert()
#include <cstdint>  // for int64_t
#include <string>

#include "../../core/client/channel_sptr.h"           // for ChannelSptr
#include "../../core/client/close_cb.h"               // for CloseCb
#include "../../core/client/impl/client_async_writer_impl_wrapper.h"  // for ClientAsyncWriterImplWrapper
#include "../common/completion_queue_sptr.h"  // for CompletionQueueSptr

namespace core {

ClientAsyncWriter::ClientAsyncWriter(const ChannelSptr& channel,
                                     const std::string& method,
                                     const CompletionQueueSptr& cq_sptr,
                                     int64_t timeout_ms)
    // Todo: same as ClientReader?
    : impl_sptr_(
          new ClientAsyncWriterImplWrapper(channel, method, cq_sptr, timeout_ms)) {
  assert(channel);
  assert(cq_sptr);
}

// Todo: Get queue size()
// Todo: SyncGetInitMd();

bool ClientAsyncWriter::Write(const std::string& request) const {
  return impl_sptr_->Write(request);
}

void ClientAsyncWriter::Close(const CloseCb& close_cb/* = nullptr*/) {
  impl_sptr_->Close(close_cb);
}  // Close()

}  // namespace core
