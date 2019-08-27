// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_SERVER_SERVER_READER_CQTAG_H
#define CORE_IMPL_SERVER_SERVER_READER_CQTAG_H

#include <functional>
#include <memory>  // for shared_ptr<>
#include <string>

#include <grpc/support/port_platform.h>    // for GRPC_MUST_USE_RESULT

#include "../../../../core/common/impl/call.h"                    // for StartBatch()
#include "../../../../core/common/impl/cqtag/call_cqtag.h"        // for CallCqTag
#include "../../../../core/common/support/config.h"  // for GRPC_FINAL
#include "../../server_reader.h"  // for ServerReader
#include "../../../common/impl/call_op_data.h"            // for CodSendInitMd
#include "../../../common/impl/call_operations.h"         // for CallOperations

namespace core {

class ServerReaderCqTag GRPC_FINAL : public CallCqTag {
 public:
  using Reader = ServerReader;
  using ReaderSptr = std::shared_ptr<Reader>;
  inline explicit ServerReaderCqTag(const CallSptr& call_sptr,
                                    const ReaderSptr& reader_sptr);

 public:
  inline bool Start() GRPC_MUST_USE_RESULT;

 public:
  inline void DoComplete(bool success) GRPC_OVERRIDE;

 private:
  inline void EndOnError(const Status& status);

 private:
  CodRecvMsg cod_recv_msg_;
  ReaderSptr reader_sptr_;
};  // class ServerReaderCqTag

ServerReaderCqTag::ServerReaderCqTag(
    const CallSptr& call_sptr, const ReaderSptr& reader_sptr)
    : CallCqTag(call_sptr), reader_sptr_(reader_sptr) {
  assert(call_sptr);
  assert(reader_sptr);
}

bool ServerReaderCqTag::Start() {
  CallOperations ops;
  ops.RecvMsg(cod_recv_msg_);
  return GetCallSptr()->StartBatch(ops, this);
}

void ServerReaderCqTag::DoComplete(bool success) {
  if (!success) {
    EndOnError(Status::InternalError("ServerReaderCqTag failed."));
    return;
  }
  if (!cod_recv_msg_.HasGotMsg()) {
    reader_sptr_->OnEnd();  // Normal end.
    return;
  }

  std::string msg;
  Status status = cod_recv_msg_.GetResultMsg(msg);
  if (!status.ok()) {
    EndOnError(status);
    return;
  }
  status = reader_sptr_->OnMsgStr(msg);
  if (!status.ok()) {
    EndOnError(status);
    return;
  }

  const CallSptr& call_sptr = GetCallSptr();
  auto* tag = new ServerReaderCqTag(call_sptr, reader_sptr_);
  if (tag->Start()) return;
  delete tag;
  EndOnError(Status::InternalError("Failed to read client-side streaming."));
}

void ServerReaderCqTag::EndOnError(const Status& status)
{
  assert(!status.ok());
  reader_sptr_->OnError(status);
  reader_sptr_->OnEnd();
}

}  // namespace core

#endif  // CORE_IMPL_SERVER_SERVER_READER_CQTAG_H
