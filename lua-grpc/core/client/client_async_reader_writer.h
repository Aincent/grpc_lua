// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_ASYNC_READER_WRITER_H
#define CORE_CLIENT_ASYNC_READER_WRITER_H

#include <cstdint>  // for int64_t
#include <string>

#include "../../core/client/channel_sptr.h"           // for ChannelSptr
#include "../../core/client/msg_str_cb.h"             // for MsgStrCb
#include "../../core/client/status_cb.h"              // for StatusCb
#include "../../core/common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../core/common/support/config.h"         // for GRPC_FINAL

namespace core {

class ClientAsyncReaderWriterImplWrapper;

// Copyable. Thread-safe.
class ClientAsyncReaderWriter GRPC_FINAL {
 public:
  // Todo: Move status_cb to Set()
  ClientAsyncReaderWriter(const ChannelSptr& channel, const std::string& method,
                          const CompletionQueueSptr& cq_sptr,
                          int64_t timeout_ms,
                          const StatusCb& status_cb = nullptr);

 public:
  bool Write(const std::string& request) const;

  // Optional. Writing is auto closed in dtr().
  // Redundant calls are ignored.
  void CloseWriting();

  void ReadEach(const MsgStrCb& msg_cb);

  // Set error status to break reading. Such as when parsing message failed.
  void SetErrorStatus(const Status& error_status);

 private:
  using Impl = ClientAsyncReaderWriterImplWrapper;
  const std::shared_ptr<Impl> impl_sptr_;
};  // class ClientAsyncReaderWriter

}  // namespace core

#endif  // CORE_CLIENT_ASYNC_READER_WRITER_H
