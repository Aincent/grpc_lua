// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_READER_ASYNC_RECV_STATUS_CQTAG_H
#define CORE_CLIENT_READER_ASYNC_RECV_STATUS_CQTAG_H

#include "../../../../core/client/impl/cqtag/client_reader_recv_status_cqtag.h"  // for ClientReaderRecvStatusCqTag
#include "../../status_cb.h"  // for StatusCb
#include "../../../common/call_sptr.h"   // for CallSptr
#include "../../../common/support/config.h"   // for GRPC_FINAL

namespace core {

// Recv status asynchronously for ClientReader and ClientReaderWriter.
class ClientReaderAsyncRecvStatusCqTag GRPC_FINAL
    : public ClientReaderRecvStatusCqTag {
 public:
  explicit ClientReaderAsyncRecvStatusCqTag(const CallSptr& call_sptr)
      : ClientReaderRecvStatusCqTag(call_sptr) {
    assert(call_sptr);
  }

  void SetStatusCb(const StatusCb& status_cb) {
    status_cb_ = status_cb;
  }

 public:
  inline void DoComplete(bool success) GRPC_OVERRIDE;

 private:
  StatusCb status_cb_;
};  // class ClientReaderAsyncRecvStatusCqTag

void ClientReaderAsyncRecvStatusCqTag::DoComplete(bool success) {
  if (!status_cb_) return;
  if (success) {
    status_cb_(GetStatus());
    return;
  }
  status_cb_(Status::InternalError("Failed to receive status."));
}

}  // namespace core
#endif  // CORE_CLIENT_READER_ASYNC_RECV_STATUS_CQTAG_H
