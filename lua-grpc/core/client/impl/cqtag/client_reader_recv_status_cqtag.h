// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_READER_RECV_STATUS_CQTAG_H
#define CORE_CLIENT_READER_RECV_STATUS_CQTAG_H

#include <grpc/support/port_platform.h>  // for GRPC_MUST_USE_RESULT

#include "../../../common/call_sptr.h"  // for CallSptr
#include "../../../common/impl/call.h"               // for StartBatch()
#include "../../../common/impl/call_op_data.h"       // for CodClientRecvStatus
#include "../../../common/impl/call_operations.h"    // for CallOperations
#include "../../../common/impl/cqtag/call_cqtag.h"   // for CallCqTag

namespace core {

// Recv status for ClientReader and ClientReaderWriter.
// Base class of ClientReaderAsyncReadStatusCqTag.
class ClientReaderRecvStatusCqTag : public CallCqTag {
 public:
  explicit ClientReaderRecvStatusCqTag(const CallSptr& call_sptr)
      : CallCqTag(call_sptr) {
    assert(call_sptr);
  }
  virtual ~ClientReaderRecvStatusCqTag() {}

 public:
  inline bool Start() GRPC_MUST_USE_RESULT;

public:
  Status GetStatus() {
    // Todo: Get trailing metadata.
    return cod_client_recv_status_.GetStatus();
  }

 private:
  CodClientRecvStatus cod_client_recv_status_;
};  // class ClientReaderRecvStatusCqTag

bool ClientReaderRecvStatusCqTag::Start() {
  CallOperations ops;
  ops.ClientRecvStatus(cod_client_recv_status_);
  return GetCallSptr()->StartBatch(ops, this);
}

}  // namespace core
#endif  // CORE_CLIENT_READER_RECV_STATUS_CQTAG_H
