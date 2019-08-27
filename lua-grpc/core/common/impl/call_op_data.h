#ifndef CORE_IMPL_CALL_OP_DATA_H
#define CORE_IMPL_CALL_OP_DATA_H

#include <cassert>

#include <grpc/slice.h>                  // for grpc_slice
#include <grpc/support/alloc.h>          // for gpr_free()
#include <grpc/support/port_platform.h>  // for GRPC_MUST_USE_RESULT

#include "../../../core/common/impl/metadata_vector.h"  // for MetadataVector
#include "../../../core/common/impl/noncopyable.h"   // for noncopyable
#include "../../../core/common/status.h"                // for Status
#include "../support/config.h"        // for GRPC_FINAL

namespace core {

// Call operation data (Cod) classes.
// Call operation data need to be kept in CallCqTag instead of CallOperations,
//   because CallOperations object is transient.

/*
CallOperations object has 7 operations, and need 6 Cod:
  SendInitMd(MetadataVector&)
  SendMsg(const string& msg, CodSendMsg&)
  RecvInitMd(CodRecvInitMd&)
  RecvMsg(CodRecvMsg&)
  ClientSendClose() (do not need Cod)
  ClientRecvStatus(CodClientRecvStatus&)
  ServerSendStatus(CodServerSendStatus&)
*/
class CodSendInitMd;
class CodSendMsg;
class CodRecvInitMd;
class CodRecvMsg;
class CodClientRecvStatus;
class CodServerSendStatus;

// Cod to send initial metadata.
class CodSendInitMd GRPC_FINAL {
 public:
  // Todo: Set metadata.
  MetadataVector& GetMdVec() { return init_md_vec_; }

 private:
  // MetadataVector is only references of keys and values.
  // The key-value strings must live until completion.
  // Keeping metadata key-values outside is because they are usually const.
  // Variable metadata can be kept in CallCqTag.
  MetadataVector init_md_vec_;
};  // class CodSendInitMd

// Cod to send message.
class CodSendMsg GRPC_FINAL : noncopyable {
 public:
  ~CodSendMsg() {
    grpc_byte_buffer_destroy(send_buf_);
  }

  void CopyMsgStr(const std::string& sMsg);

  grpc_byte_buffer* GetSendBuf() const { return send_buf_; }

 private:
  // send_buf_ is created in CopyMsgStr() and destroyed in dtr().
  grpc_byte_buffer* send_buf_ = nullptr;  // owned
  // Todo: WriteOptions write_options_;
  //   or outside in CallOperations::SendMsg()?
};  // class CodSendMsg

// Cod to receive initial metadata.
class CodRecvInitMd GRPC_FINAL : noncopyable {
 public:
  CodRecvInitMd() {
    grpc_metadata_array_init(&recv_init_md_arr_);
  }
  ~CodRecvInitMd() {
    grpc_metadata_array_destroy(&recv_init_md_arr_);
  }

  grpc_metadata_array* GetRecvInitMdArrPtr() { return &recv_init_md_arr_; }
  // Todo: Get result metadata.

 private:
  // std::multimap<grpc::string_ref, grpc::string_ref>* recv_init_md_;
  grpc_metadata_array recv_init_md_arr_;
};  // class CodRecvInitMd

// Cod to receive message.
class CodRecvMsg GRPC_FINAL : noncopyable {
 public:
  ~CodRecvMsg() {
    grpc_byte_buffer_destroy(recv_buf_);
  }
  grpc_byte_buffer** GetRecvBufPtr() { return &recv_buf_; }

  // To detect end of stream.
  bool HasGotMsg() const { return nullptr != recv_buf_; }
  Status GetResultMsg(std::string& result) const;

 private:
  grpc_byte_buffer* recv_buf_ = nullptr;  // owned
};  // class CodRecvMsg

// No Cod for ClientSendClose

// Cod for client to receive status.
class CodClientRecvStatus : noncopyable {
 public:
  CodClientRecvStatus() : status_details_(grpc_empty_slice()) {
    grpc_metadata_array_init(&recv_trail_md_arr_);
  }
  ~CodClientRecvStatus() {
    grpc_metadata_array_destroy(&recv_trail_md_arr_);
    grpc_slice_unref(status_details_);
  }

 public:
  grpc_metadata_array* GetTrailMdArrPtr() { return &recv_trail_md_arr_; }
  grpc_status_code* GetStatusCodePtr() { return &status_core_; }
  grpc_slice* GetStatusDetailsPtr() { return &status_details_; }

 public:
  bool IsStatusOk() const { return status_core_ == GRPC_STATUS_OK; }
  Status GetStatus() const;

 private:
  // Todo: std::multimap<grpc::string_ref, grpc::string_ref>* recv_trailing_metadata_;
  // Todo: Status* recv_status_ = nullptr;

  // Metadata array to receive trailing metadata.
  grpc_metadata_array recv_trail_md_arr_;
  grpc_status_code status_core_ = GRPC_STATUS_OK;
  grpc_slice status_details_;
};

// Cod for server to send status.
class CodServerSendStatus GRPC_FINAL {
 public:
  void SetStatus(const Status& status) {
    // Should set only once.
    assert(GRPC_STATUS_OK == send_status_core_);
    assert(send_status_details_.empty());
    send_status_core_ = status.GetCode();
    send_status_details_ = status.GetDetails();
  }

  // Todo: set trailing metadata.

  grpc_metadata* GetTrailMdArrPtr() {
    return trail_md_vec_.empty() ? nullptr : &trail_md_vec_[0];
  }
  size_t GetTrailMdCount() const { return trail_md_vec_.size(); }
  grpc_status_code GetStatusCode() const { return send_status_core_; }
  const char* GetStatusDetailsBuf() const { return send_status_details_.c_str(); }

 private:
  grpc_status_code send_status_core_ = GRPC_STATUS_OK;
  std::string send_status_details_;
  // Trailing metadata.
  MetadataVector trail_md_vec_;
};  // class CodServerSendStatus

}  // namespace core
#endif  // CORE_IMPL_CALL_OP_DATA_H
