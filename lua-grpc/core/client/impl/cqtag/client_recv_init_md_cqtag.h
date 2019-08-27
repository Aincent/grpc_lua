// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_CLIENT_CLIENT_RECV_INIT_MD_CQTAG_H
#define CORE_IMPL_CLIENT_CLIENT_RECV_INIT_MD_CQTAG_H

#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../common/impl/call.h"              // for StartBatch()
#include "../../../common/impl/call_op_data.h"      // for CodRecvSendInitMd
#include "../../../common/impl/call_operations.h"   // for CallOperations
#include "../../../common/impl/cqtag/call_cqtag.h"  // for CallCqTag

namespace core {

// Receive init metadata.
// Used in ClientReaderWriter.
class ClientRecvInitMdCqTag : public CallCqTag {
 public:
  inline explicit ClientRecvInitMdCqTag(const CallSptr& call_sptr) : CallCqTag(call_sptr) {}
  inline virtual ~ClientRecvInitMdCqTag() {}
  inline bool Start() GRPC_MUST_USE_RESULT;

 private:
  CodRecvInitMd cod_recv_init_md_;
};  // class ClientRecvInitMdCqTag

bool ClientRecvInitMdCqTag::Start() {
  CallOperations ops;
  ops.RecvInitMd(cod_recv_init_md_);
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core

#endif  // CORE_IMPL_CLIENT_CLIENT_RECV_INIT_MD_CQTAG_H
