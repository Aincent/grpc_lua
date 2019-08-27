// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_CLIENT_CLIENT_SEND_MSG_CQTAG_H
#define CORE_IMPL_CLIENT_CLIENT_SEND_MSG_CQTAG_H

#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../common/support/config.h"    // for GRPC_FINAL
#include "../../../common/impl/call.h"                      // for StartBatch()
#include "../../../common/impl/call_op_data.h"              // for CodSendMsg
#include "../../../common/impl/call_operations.h"           // for CallOperations
#include "../../../common/impl/cqtag/general_call_cqtag.h"  // for GeneralCallCqTag

namespace core {

class ClientSendMsgCqTag GRPC_FINAL : public GeneralCallCqTag {
 public:
  explicit ClientSendMsgCqTag(const CallSptr& call_sptr)
      : GeneralCallCqTag(call_sptr) {}

 public:
  inline bool Start(const std::string& message) GRPC_MUST_USE_RESULT;

 private:
  CodSendMsg cod_send_msg_;
};  // class ClientSendMsgCqTag

bool ClientSendMsgCqTag::Start(const std::string& message) {
  CallOperations ops;
  ops.SendMsg(message, cod_send_msg_);  // XXX check result
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core

#endif  // CORE_IMPL_CLIENT_CLIENT_SEND_MSG_CQTAG_H
