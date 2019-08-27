// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_IMPL_CLIENT_ASYNC_WRITER_IMPL_H
#define CORE_CLIENT_IMPL_CLIENT_ASYNC_WRITER_IMPL_H

#include <cstdint>  // for int64_t
#include <memory>  // for enable_shared_from_this<>
#include <mutex>
#include <string>
#include <queue>

#include "../../../core/client/channel_sptr.h"           // for ChannelSptr
#include "../../../core/client/close_cb.h"               // for CloseCb
#include "../../../core/common/call_sptr.h"              // for CallSptr
#include "../../../core/common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../../core/common/status.h"                 // for Status
#include "../../../core/common/support/config.h"         // for GRPC_FINAL

namespace core {

class ClientWriterCloseCqTag;

// ImplWrapper is to make Writer copyable and call Close() in dtr().
// Impl will live longer than the ImplWrapper.
// Thread-safe.
class ClientAsyncWriterImpl GRPC_FINAL
    : public std::enable_shared_from_this<ClientAsyncWriterImpl> {
 public:
  ClientAsyncWriterImpl(const ChannelSptr& channel, const std::string& method,
                         const CompletionQueueSptr& cq_sptr, int64_t timeout_ms);
  ~ClientAsyncWriterImpl();

  bool Write(const std::string& request);
  void Close(const CloseCb& close_cb = nullptr);

  // Todo: Force to close, cancel all writing.
  // Todo: get queue size

 private:
  // for ClientWriterCloseCqTag::OnComplete()
  void OnClosed(bool success, ClientWriterCloseCqTag& tag);
  // for ClientSendMsgCqTag::OnComplete()
  void OnSent(bool success);

 private:
  void SendClose();
  void TryToCallCloseCb();  // always on error
  void CallCloseCb(const std::string& sMsg = "");
  bool SendNext();

 private:
  // The callback may lock the mutex recursively.
  using Mutex = std::recursive_mutex;
  mutable Mutex mtx_;
  using Guard = std::lock_guard<Mutex>;

  const CallSptr call_sptr_;
  Status status_;

#ifndef NDEBUG
  bool has_sent_close_ = false;  // Client send close once.
#endif  // NDEBUG

  CloseCb close_cb_;         // Set by Close() once
  bool is_closing_ = false;  // Set by Close() once

  // Grpc only allows to write one by one, so queue messages before write.
  // The front message is sending.
  std::queue<std::string> msg_queue_;
};  // class ClientAsyncWriterImpl

}  // namespace core
#endif  // CORE_CLIENT_IMPL_CLIENT_ASYNC_WRITER_IMPL_H
