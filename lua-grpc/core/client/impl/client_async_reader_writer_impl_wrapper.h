// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_IMPL_CLIENT_ASYNC_READER_WRITER_IMPL_WRAPPER_H
#define CORE_CLIENT_IMPL_CLIENT_ASYNC_READER_WRITER_IMPL_WRAPPER_H

#include <string>

#include "../channel_sptr.h"           // for ChannelSptr
#include "../msg_str_cb.h"             // for MsgStrCb
#include "../status_cb.h"              // for StatusCb
#include "../../common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../common/support/config.h"         // for GRPC_FINAL

namespace core {

class ClientAsyncReaderWriterImpl;
class Status;

// Thread-safe.
// Only shared in ClientAsyncReaderWriter, because we need dtr() to close writing.
class ClientAsyncReaderWriterImplWrapper GRPC_FINAL {
 public:
  ClientAsyncReaderWriterImplWrapper(const ChannelSptr& channel,
                                     const std::string& method,
                                     const CompletionQueueSptr& cq_sptr,
                                     int64_t timeout_ms,
                                     const StatusCb& status_cb);
  ~ClientAsyncReaderWriterImplWrapper();

 public:
  bool Write(const std::string& msg);
  // Optional. Auto closed in dtr().
  void CloseWriting();

  // Todo: Force to close reading/writing. Cancel all reading/writing.

  void ReadEach(const MsgStrCb& msg_cb);

  // Set error status to break reading. Such as when parsing message failed.
  void SetErrorStatus(const Status& error_status);

 private:
  // Live longer than ClientAsyncReaderWriter.
  std::shared_ptr<ClientAsyncReaderWriterImpl> impl_sptr_;
};  // class ClientAsyncReaderWriterImplWrapper

// Todo: SyncGetInitMd();

}  // namespace core
#endif  // CORE_CLIENT_IMPL_CLIENT_ASYNC_READER_WRITER_IMPL_WRAPPER_H
