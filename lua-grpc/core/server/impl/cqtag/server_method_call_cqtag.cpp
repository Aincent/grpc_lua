// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#include "../../../../core/server/impl/cqtag/server_method_call_cqtag.h"

#include "../../../../core/common/completion_queue_for_next.h" // for c_cq()
#include "../../../../core/common/impl/call.h"             // for Call
#include "../../../../core/server/service.h"  // for Service

namespace core {

ServerMethodCallCqTag::ServerMethodCallCqTag(
    grpc_server* server, Service* service, size_t method_index,
    void* registered_method, const CompletionQueueForNextSptr& cq4n_sptr)
    : server_(server),
      service_(service),
      method_index_(method_index),
      registered_method_(registered_method),
      cq4n_sptr_(cq4n_sptr),
      call_ptr_(nullptr),
      deadline_(gpr_inf_future(GPR_CLOCK_REALTIME)),
      payload_ptr_(nullptr) {
  assert(server);
  assert(registered_method);
  assert(cq4n_sptr);
  assert(method_index < service->GetMethodCount());

  memset(&initial_metadata_array_, 0, sizeof(initial_metadata_array_));
  grpc_completion_queue* ccq = &cq4n_sptr->c_cq();
  grpc_byte_buffer** optional_payload =
      service->IsMethodClientStreaming(method_index) ? nullptr : &payload_ptr_;
  grpc_call_error ret = grpc_server_request_registered_call(
      server, registered_method, &call_ptr_, &deadline_,
      &initial_metadata_array_, optional_payload, ccq, ccq, this);
  assert(GRPC_CALL_OK == ret);
}

ServerMethodCallCqTag::~ServerMethodCallCqTag() {
  grpc_metadata_array_destroy(&initial_metadata_array_);
  grpc_byte_buffer_destroy(payload_ptr_);
}

void ServerMethodCallCqTag::DoComplete(bool success)
{
  if (success) {
    // Deal payload.
    assert(service_);
    assert(call_ptr_);
    CallSptr call_sptr(new Call(call_ptr_));  // destroys grpc_call
    service_->CallMethod(method_index_, payload_ptr_, call_sptr);
  }

  // Request the next method call.
  // Calls grpc_server_request_registered_call() in ctr().
  // Delete in Server::Run().
  new ServerMethodCallCqTag(server_, service_, method_index_,
                            registered_method_, cq4n_sptr_);
}

}  // namespace core
