// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_CLIENT_WRITER_CLOSE_CQTAG_H
#define CORE_CLIENT_CLIENT_WRITER_CLOSE_CQTAG_H

#include <string>
#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../common/support/config.h"    // for GRPC_FINAL
#include "../../../common/impl/call.h"                      // for StartBatch()
#include "../../../common/impl/call_op_data.h"              // for CodRecvMsg
#include "../../../common/impl/call_operations.h"           // for CallOperations
#include "../../../common/impl/cqtag/general_call_cqtag.h"  // for GeneralCallCqTag

namespace core {

class ClientWriterCloseCqTag GRPC_FINAL : public GeneralCallCqTag {
 public:
  explicit ClientWriterCloseCqTag(const CallSptr& call_sptr)
      : GeneralCallCqTag(call_sptr) {}

  inline bool Start() GRPC_MUST_USE_RESULT;
  inline bool IsStatusOk() const {
    return cod_client_recv_status_.IsStatusOk();
  }
  inline Status GetStatus() const {
    return cod_client_recv_status_.GetStatus();
  }
  inline Status GetResponse(std::string& response) GRPC_MUST_USE_RESULT {
    return cod_recv_msg_.GetResultMsg(response);  // XXX no use?, GetCallSptr()->GetMaxMsgSize());
  }

 private:
  CodRecvInitMd cod_recv_init_md_;
  CodRecvMsg cod_recv_msg_;
  CodClientRecvStatus cod_client_recv_status_;
};  // class ClientWriterCloseCqTag

bool ClientWriterCloseCqTag::Start() {
  CallOperations ops;
  ops.ClientSendClose();  // TODO: separate send and recv
  ops.RecvInitMd(cod_recv_init_md_);
  ops.RecvMsg(cod_recv_msg_);
  ops.ClientRecvStatus(cod_client_recv_status_);
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core

#endif  // CORE_CLIENT_CLIENT_WRITER_CLOSE_CQTAG_H
