// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_IMPL_CLIENT_ASYNC_WRITER_IMPL_WRAPPER_H
#define CORE_CLIENT_IMPL_CLIENT_ASYNC_WRITER_IMPL_WRAPPER_H

#include <string>

#include "../channel_sptr.h"           // for ChannelSptr
#include "../close_cb.h"               // for CloseCb
#include "../../common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../common/support/config.h"         // for GRPC_FINAL

namespace core {

class ClientAsyncWriterImpl;

// Only shared in ClientAsyncWriter, because we need dtr() to close writing.
// Thread-safe.
class ClientAsyncWriterImplWrapper GRPC_FINAL {
 public:
  ClientAsyncWriterImplWrapper(const ChannelSptr& channel,
                               const std::string& method,
                               const CompletionQueueSptr& cq_sptr,
                               int64_t timeout_ms);
  ~ClientAsyncWriterImplWrapper();

  bool Write(const std::string& request);
  void Close(const CloseCb& close_cb);

  // Todo: Force to close, cancel all writing. CloseNow()
  // Todo: get queue size

 private:
  // Will live longer than ClientAsyncWriter.
  std::shared_ptr<ClientAsyncWriterImpl> impl_sptr_;
};  // class ClientAsyncWriterImplWrapper

}  // namespace core
#endif  // CORE_CLIENT_IMPL_CLIENT_ASYNC_WRITER_IMPL_WRAPPER_H
