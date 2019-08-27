// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_CLIENT_CLIENT_ASYNC_READER_IMPL_H
#define CORE_IMPL_CLIENT_CLIENT_ASYNC_READER_IMPL_H

#include <memory>  // for enable_shared_from_this<>
#include <mutex>
#include <string>

#include "../../../core/common/call_sptr.h"              // for CallSptr
#include "../../../core/common/status.h"                 // for Status
#include "../channel_sptr.h"           // for ChannelSptr
#include "../msg_str_cb.h"             // for MsgStrCb
#include "../status_cb.h"              // for StatusCb
#include "../../common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../common/support/config.h"         // for GRPC_FINAL

namespace core {

class ClientAsyncReadWorker;
class ClientReaderReadCqTag;

// Thread-safe.
class ClientAsyncReaderImpl GRPC_FINAL
    : public std::enable_shared_from_this<ClientAsyncReaderImpl> {
 public:
  ClientAsyncReaderImpl(const ChannelSptr& channel, const std::string& method,
      const std::string& request, const CompletionQueueSptr& cq_sptr,
      int64_t timeout_ms);
  ~ClientAsyncReaderImpl();

 public:
  void Start(const MsgStrCb& msg_cb = nullptr,
      const StatusCb& status_cb = nullptr);

  // Todo: Stop reading any more...

 private:
  void OnRead(bool success, ClientReaderReadCqTag& tag);

 private:
  bool Init();
  void ReadNext();  // Setup next async read.
  void CallStatusCb();

 private:
  // OnRead() may lock again.
  using Mutex = std::recursive_mutex;
  Mutex mtx_;
  using Guard = std::lock_guard<Mutex>;

  const CallSptr call_sptr_;
  const std::string request_;
  Status status_;
  MsgStrCb msg_cb_;
  StatusCb status_cb_;
  bool is_started_ = false;
};  // class ClientAsyncReaderImpl

}  // namespace core
#endif  // CORE_IMPL_CLIENT_CLIENT_ASYNC_READER_IMPL_H
