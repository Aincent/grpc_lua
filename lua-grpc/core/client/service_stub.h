// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)
#ifndef CORE_CLIENT_SERVICE_STUB_H
#define CORE_CLIENT_SERVICE_STUB_H

#include <atomic>  // for atomic<>
#include <cassert>  // for assert()

#include "../../core/client/channel.h"  // for GetCallTimeoutMs()
#include "../../core/client/channel_sptr.h"     // for ChannelSptr
#include "../../core/client/msg_str_cb.h"     // for RespStrCb
#include "../../core/client/status_cb.h"  // for ErrorCb
#include "../../core/client/status_cb.h"  // for ErrorCb
#include "../../core/client/msg_bool_int_cb.h" // for WFSCb
#include "../../core/common/completion_queue_for_next_sptr.h"  // for CompletionQueueForNextSptr
#include "../../core/common/support/core_api.h"  // for CORE_API

namespace core {

class CompletionQueue;

// The base of generated service stubs.
// Copyable.
// Thread-safe except for set methods.
class CORE_API ServiceStub {
 public:
  explicit ServiceStub(const ChannelSptr& channel_sptr,
      const CompletionQueueForNextSptr& cq4n_sptr = nullptr);
  virtual ~ServiceStub();

 public:
  using string = std::string;

 public:
  Channel& GetChannel() const {
    assert(channel_sptr_);
    return *channel_sptr_;
  }
  // Non-null channel sptr.
  ChannelSptr GetChannelSptr() const { return channel_sptr_; }

  const StatusChangeCb& GetStatusChangeCb() const {
    return status_change_cb_;
  }
  // non-thread-safe
  void SetStatusChangeCb(const StatusChangeCb& status_change_cb) {
	  status_change_cb_ = status_change_cb;
	  if(!start_)
	  {
		  start_ = true;
		  pthread_create(m_pID, NULL, &ThreadRun, this);
	  }
  }

  const ErrorCb& GetErrorCb() const {
    return error_cb_;
  }
  // non-thread-safe
  void SetErrorCb(const ErrorCb& error_cb) {
    error_cb_ = error_cb;
  }

  CompletionQueueForNextSptr GetCompletionQueue() const {
    assert(cq4n_sptr_);
    return cq4n_sptr_;
  }
  // non-thread-safe
  void SetCompletionQueue(const CompletionQueueForNextSptr& cq4n_sptr) {
    if (cq4n_sptr)
      cq4n_sptr_ = cq4n_sptr;
  }

  // ServiceStub can set timeout for all methods calls.
  int64_t GetCallTimeoutMs() const { return *call_timeout_ms_sptr_; }
  void SetCallTimeoutMs(int64_t timeout_ms) {
    *call_timeout_ms_sptr_ = timeout_ms;
  }

  grpc_connectivity_state GetChannelStatus();

 public:
  static ErrorCb& GetDefaultErrorCb() {
    return default_error_cb_;
  }
  // non-thread-safe
  static void SetDefaultErrorCb(const ErrorCb& error_cb) {
    default_error_cb_ = error_cb;
  }

 public:
  Status SyncRequest(const string& method, const string& request,
                     string& response);

  void AsyncRequest(const string& method, const string& request,
                    const RespStrCb& response_cb,
                    const ErrorCb& error_cb = GetDefaultErrorCb());

 public:
  void Run();
  // Request the shutdown of all runs.
  void Shutdown();

  static void *ThreadRun(void *arg);

 protected:
  CallSptr MakeSharedCall(const string& method) const;
  CallSptr MakeSharedCall(const string& method, CompletionQueue& cq) const;

 private:
  bool start_;
  ChannelSptr channel_sptr_;
  CompletionQueueForNextSptr cq4n_sptr_;

  StatusChangeCb status_change_cb_; //callback on wait for status change
  ErrorCb error_cb_;  // callback on error
  std::shared_ptr<std::atomic<int64_t>> call_timeout_ms_sptr_;  // copyable

  pthread_t *m_pID;


 private:
  static ErrorCb default_error_cb_;
};  // class ServiceStub

static_assert(std::is_copy_constructible<ServiceStub>::value,
              "ServiceStub should be copyable");
static_assert(std::is_copy_assignable<ServiceStub>::value,
              "ServiceStub should be copyable");

}  // namespace core
#endif  // CORE_CLIENT_SERVICE_STUB_H
