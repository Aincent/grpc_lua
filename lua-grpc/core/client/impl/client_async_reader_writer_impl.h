// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_IMPL_CLIENT_ASYNC_READER_WRITER_IMPL_H
#define CORE_CLIENT_IMPL_CLIENT_ASYNC_READER_WRITER_IMPL_H

#include <cassert>
#include <cstdint>  // for int64_t
#include <memory>  // for enable_shared_from_this<>
#include <mutex>
#include <string>
#include <queue>

#include "../../../core/client/channel_sptr.h"           // for ChannelSptr
#include "../../../core/client/msg_str_cb.h"             // for MsgStrCb
#include "../../../core/client/status_cb.h"              // for StatusCb
#include "../../../core/common/call_sptr.h"              // for CallSptr
#include "../../../core/common/completion_queue_sptr.h"  // for CompletionQueueSptr
#include "../../../core/common/status.h"                 // for Status
#include "../../../core/common/support/config.h"         // for GRPC_FINAL

namespace core {

class ClientReaderReadCqTag;

// ImplWrapper is to make ReaderWriter copyable and call CloseWriting() in dtr().
// Impl will live longer than the ImplWrapper.
// Thread-safe.
class ClientAsyncReaderWriterImpl GRPC_FINAL
    : public std::enable_shared_from_this<ClientAsyncReaderWriterImpl> {
 public:
  ClientAsyncReaderWriterImpl(const ChannelSptr& channel,
                               const std::string& method,
                               const CompletionQueueSptr& cq_sptr,
                               int64_t timeout_ms,
                               const StatusCb& status_cb);
  ~ClientAsyncReaderWriterImpl();

 public:
  bool Write(const std::string& msg);
  void CloseWriting();

  // Todo: Force to close reading/writing. Cancel all reading/writing.

  void ReadEach(const MsgStrCb& msg_cb);

  // Set error status to break reading. Such as when parsing message failed.
  void SetErrorStatus(const Status& error_status);

 private:
  // For ClientSendMsgCqTag.
  void OnSent(bool success);
  // For ClientReaderReadCqTag.
  void OnRead(bool success, ClientReaderReadCqTag& tag);

 private:
  void InitIfNot();
  bool SendNext();
  void ReadNext();
  void SendClose();
  void SetInternalError(const std::string& sError);
  void EndOnErrorStatus(const Status& error_status);
  void CallStatusCb();

 private:
  // Callbacks will lock again.
  using Mutex = std::recursive_mutex;
  mutable Mutex mtx_;
  using Guard = std::lock_guard<Mutex>;

  const CallSptr call_sptr_;
  Status status_;
  MsgStrCb msg_cb_;
  StatusCb status_cb_;
  bool inited_ = false;  // Init once.
  bool reading_started_ = false;  // Set by ReadEach()
  bool writing_closing_ = false;  // Set by CloseWriting()
  bool reading_ended_ = false;
  bool writing_ended_ = false;
  std::queue<std::string> msg_queue_;  // Cache messages to write.

#ifndef NDEBUG
  bool has_sent_close_ = false;  // Client send close once.
#endif  // NDEBUG
};  // class ClientAsyncReaderWriterImpl

// Todo: SyncGetInitMd();

}  // namespace core
#endif  // CORE_CLIENT_IMPL_CLIENT_ASYNC_READER_WRITER_IMPL_H
