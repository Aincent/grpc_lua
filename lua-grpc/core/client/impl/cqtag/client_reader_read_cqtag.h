// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_CLIENT_READER_READ_CQTAG_H
#define CORE_CLIENT_CLIENT_READER_READ_CQTAG_H

#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../../core/common/impl/call.h"                      // for GetMaxMsgSize()
#include "../../../../core/common/impl/cqtag/general_call_cqtag.h"  // for GeneralCallCqTag
#include "../../../common/support/config.h"    // for GRPC_FINAL
#include "../../../common/impl/call_op_data.h"              // for CodRecvMsg
#include "../../../common/impl/call_operations.h"           // for CallOperations

namespace core {

// Completion queue tag to read msg.
// For ClientReader and ClientReaderWriter.
class ClientReaderReadCqTag GRPC_FINAL : public GeneralCallCqTag {
 public:
  explicit ClientReaderReadCqTag(const CallSptr& call_sptr)
      : GeneralCallCqTag(call_sptr) {}

  inline bool Start() GRPC_MUST_USE_RESULT;

  // To detect end of stream.
  inline bool HasGotMsg() const { return cod_recv_msg_.HasGotMsg(); }
  inline Status GetResultMsg(std::string& message) GRPC_MUST_USE_RESULT {
    return cod_recv_msg_.GetResultMsg(message);
    // XXX no use? GetCallSptr()->GetMaxMsgSize());
  }

 private:
  CodRecvMsg cod_recv_msg_;
};  // class ClientReaderReadCqTag

bool ClientReaderReadCqTag::Start() {
  CallOperations ops;
  ops.RecvMsg(cod_recv_msg_);
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core

#endif  // CORE_CLIENT_CLIENT_READER_READ_CQTAG_H
