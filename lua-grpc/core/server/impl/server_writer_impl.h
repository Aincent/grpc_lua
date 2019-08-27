// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_SERVER_SERVER_WRITER_IMPL_H
#define CORE_SERVER_SERVER_WRITER_IMPL_H

#include <limits>  // for numeric_limits<>
#include <memory>  // for enable_shared_from_this<>
#include <mutex>
#include <queue>
#include <string>

#include "../../common/call_sptr.h"					  // for CallSptr
#include "../../common/support/config.h"  			  // for GRPC_FINAL

namespace core {

class Status;

// Thread-safe.
class ServerWriterImpl GRPC_FINAL
    : public std::enable_shared_from_this<ServerWriterImpl> {
 public:
  explicit ServerWriterImpl(const CallSptr& call_sptr);
  ~ServerWriterImpl();  // blocking

 public:
  using string = std::string;
  // Write() will block if the high queue size reached.
  bool Write(const string& response);
  bool SyncWrite(const string& response);
  bool AsyncWrite(const string& response);

  size_t GetQueueSize() const {
    Guard g(mtx_);
    return queue_.size();
  }
  size_t GetHighQueueSize() const;
  void SetHighQueueSize(size_t high_queue_size);

  // Close() is optional. Dtr() will auto SyncClose().
  // Redundant Close() will be ignored.
  void SyncClose(const Status& status);
  void AsyncClose(const Status& status);
  bool IsClosed() const {
    Guard g(mtx_);
    return closed_;
  }

 private:
  void TryToWriteNext();  // for ServerWriterWriteCqTag::DoComplete()

 private:
  void SendStatus();  // to close
  bool SendMsg(const string& msg);

 private:
  CallSptr call_sptr_;
  bool closed_ = false;  // on error or done
  bool send_init_md_ = true;  // to send initial metadata once
  bool is_sending_ = false;  // grpc must send one by one

  size_t high_queue_size_ = std::numeric_limits<size_t>::max();
  std::queue<string> queue_;

  // new in SyncClose()/AsyncClose()
  std::unique_ptr<Status> close_status_uptr_;

  mutable std::mutex mtx_;
  using Guard = std::lock_guard<std::mutex>;
};  // class ServerWriterImpl

}  // namespace core

#endif  // CORE_SERVER_SERVER_WRITER_IMPL_H
