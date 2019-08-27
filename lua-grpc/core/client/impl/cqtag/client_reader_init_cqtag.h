// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_CLIENT_READER_INIT_CQTAG_H
#define CORE_CLIENT_CLIENT_READER_INIT_CQTAG_H

#include <string>
#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../common/support/config.h"  // for GRPC_FINAL
#include "../../../common/impl/call.h"                    // for StartBatch()
#include "../../../common/impl/call_op_data.h"            // for CodSendInitMd
#include "../../../common/impl/call_operations.h"         // for CallOperations
#include "../../../common/impl/cqtag/call_cqtag.h"        // for CallCqTag

namespace core {

class ClientReaderInitCqTag GRPC_FINAL : public CallCqTag {
 public:
  inline explicit ClientReaderInitCqTag(const CallSptr& call_sptr)
      : CallCqTag(call_sptr) {}
  inline bool Start(const std::string& request) GRPC_MUST_USE_RESULT;

 private:
  CodSendMsg cod_send_msg_;
  CodSendInitMd cod_send_init_md_;
  CodRecvInitMd cod_recv_init_md_;
};  // class ClientReaderInitCqTag

bool ClientReaderInitCqTag::Start(const std::string& request) {
  CallOperations ops;
  ops.SendMsg(request, cod_send_msg_);

  // Todo: Fill send_init_md_array_ -> FillMetadataVector()
  ops.SendInitMd(cod_send_init_md_);
  ops.RecvInitMd(cod_recv_init_md_);
  ops.ClientSendClose();
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core

#endif  // CORE_CLIENT_CLIENT_READER_INIT_CQTAG_H
