// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_ASYNC_CALL_CQTAG_H
#define CORE_CLIENT_ASYNC_CALL_CQTAG_H

#include <string>

#include "../../../../core/client/impl/cqtag/client_call_cqtag.h"  // for ClientCallCqTag
#include "../../msg_str_cb.h"                  // for RespStrCb
#include "../../status_cb.h"               // for ErrorCb
#include "../../../common/support/config.h"          // for GRPC_FINAL

namespace core {

// Completion queue tag (CqTag) for client async call.
// Derived from ClientCallCqTag, adding response_cb, error_cb.
class ClientAsyncCallCqTag GRPC_FINAL : public ClientCallCqTag {
 public:
  explicit ClientAsyncCallCqTag(const CallSptr call_sptr)
     : ClientCallCqTag(call_sptr) {}

 public:
  void SetResponseCb(const RespStrCb& response_cb) {
    response_cb_ = response_cb;
  }
  void SetErrorCb(const ErrorCb& error_cb) {
    error_cb_ = error_cb;
  }

 public:
  void DoComplete(bool success) GRPC_OVERRIDE {
    if (!success) {
      CallErrorCb(Status::InternalError("ClientAsyncCallCqTag failed."));
      return;
    }

    std::string resp;
    Status status = GetResponse(resp);
    if (!status.ok()) {
      CallErrorCb(status);
      return;
    }

    if (!response_cb_) return;
    status = response_cb_(resp);
    if (!status.ok()) CallErrorCb(status);
  };

 private:
  void CallErrorCb(const Status& status) const {
    if (error_cb_)
      error_cb_(status);
  }

 private:
  RespStrCb response_cb_;
  ErrorCb error_cb_;
};  // class ClientAsyncCallCqTag

}  // namespace core
#endif  // CORE_CLIENT_ASYNC_CALL_CQTAG_H
