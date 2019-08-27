// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_ASYNC_READER_H
#define CORE_CLIENT_ASYNC_READER_H

#include <cstdint>  // for int64_t
#include <string>

#include "../../core/client/channel_sptr.h"  // for ChannelSptr
#include "../../core/client/msg_str_cb.h"  // for MsgStrCb
#include "../../core/client/status_cb.h"  // for StatusCb
#include "../../core/common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../core/common/support/config.h"  // for GRPC_FINAL

namespace core {

class ClientAsyncReaderImpl;

// Copyable. Thread-safe.
class ClientAsyncReader GRPC_FINAL {
 public:
  ClientAsyncReader(const ChannelSptr& channel, const std::string& method,
                    const std::string& request,
                    const CompletionQueueSptr& cq_sptr, int64_t timeout_ms);

 public:
  void ReadEach(const MsgStrCb& msg_cb,
      const StatusCb& status_cb = nullptr) const;

 private:
  const std::shared_ptr<ClientAsyncReaderImpl> impl_sptr_;
};  // class ClientAsyncReader

}  // namespace core
#endif  // CORE_CLIENT_ASYNC_READER_H
