// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CPP_CB_SERVER_SERVER_WRITER_SEND_STATUS_CQTAT_H
#define CPP_CB_SERVER_SERVER_WRITER_SEND_STATUS_CQTAT_H

#include "../../../../core/common/impl/call.h"                    // for StartBatch()
#include "../../../../core/common/impl/cqtag/call_cqtag.h"        // for CallCqTag
#include "../../../common/call_sptr.h"			         // for CallSptr
#include "../../../common/support/config.h"  // for GRPC_FINAL
#include "../../../common/impl/call_op_data.h"            // for CodServerSendStatus
#include "../../../common/impl/call_operations.h"         // for CallOperations

namespace core {

class ServerWriterSendStatusCqTag GRPC_FINAL : public CallCqTag {
 public:
  explicit ServerWriterSendStatusCqTag(const CallSptr& call_sptr)
      : CallCqTag(call_sptr) {}

  inline bool StartSend(const Status& status, bool send_init_md) GRPC_MUST_USE_RESULT;

 private:
  CodSendInitMd cod_send_init_md_;
  CodServerSendStatus cod_server_send_status_;
};

bool ServerWriterSendStatusCqTag::StartSend(const Status& status,
    bool send_init_md) {
  CallOperations ops;
  if (send_init_md) {
    // todo: set md
    ops.SendInitMd(cod_send_init_md_);
  }
  ops.ServerSendStatus(status, cod_server_send_status_);
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core
#endif  // CPP_CB_SERVER_SERVER_WRITER_SEND_STATUS_CQTAT_H
