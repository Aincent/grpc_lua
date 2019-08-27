// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_CLIENT_CLIENT_SEND_INIT_MD_CQTAG_H
#define CORE_IMPL_CLIENT_CLIENT_SEND_INIT_MD_CQTAG_H

#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../common/impl/call.h"              // for StartBatch()
#include "../../../common/impl/call_op_data.h"      // for CodSendInitMd
#include "../../../common/impl/call_operations.h"   // for CallOperations
#include "../../../common/impl/cqtag/call_cqtag.h"  // for CallCqTag

namespace core {

// Send init metadata.
// Used in ClientWriter and ClientReaderWriter.
class ClientSendInitMdCqTag : public CallCqTag {
 public:
  inline explicit ClientSendInitMdCqTag(const CallSptr& call_sptr) : CallCqTag(call_sptr) {}
  inline virtual ~ClientSendInitMdCqTag() {}
  inline bool Start() GRPC_MUST_USE_RESULT;

 private:
  CodSendInitMd cod_send_init_md_;
};  // class ClientSendInitMdCqTag

bool ClientSendInitMdCqTag::Start() {
  CallOperations ops;
  // Todo: Fill send_init_md_array_ -> FillMetadataVector()
  ops.SendInitMd(cod_send_init_md_);
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core

#endif  // CORE_IMPL_CLIENT_CLIENT_SEND_INIT_MD_CQTAG_H
