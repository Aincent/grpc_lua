// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_SERVER_SERVER_METHOD_CALL_CQTAG_H
#define CORE_SERVER_SERVER_METHOD_CALL_CQTAG_H

#include <cassert>

#include <grpc/grpc.h>          // for grpc_metadata_array
#include <grpc/support/time.h>  // for gpr_timespec

#include "../../../../core/common/impl/cqtag/completion_queue_tag.h"  // for CompletionQueueTag
#include "../../../common/completion_queue_for_next_sptr.h"  // for CompletionQueueForNextSptr
#include "../../../common/support/config.h"     // for GRPC_FINAL

struct grpc_call;

namespace core {

class Service;

// Used in class Server.
class ServerMethodCallCqTag GRPC_FINAL : public CompletionQueueTag {
 public:
  // registered_method is the return of grpc_server_register_method().
  ServerMethodCallCqTag(grpc_server* server, Service* service,
                        size_t method_index, void* registered_method,
                        const CompletionQueueForNextSptr& cq4n_sptr);

  virtual ~ServerMethodCallCqTag() GRPC_OVERRIDE;

 public:
  virtual void DoComplete(bool success) GRPC_OVERRIDE;

 private:
  // TODO: No need to copy for each request.
  grpc_server* const server_;
  Service* const service_;
  const size_t method_index_;
  void* const registered_method_;
  const CompletionQueueForNextSptr cq4n_sptr_;

  grpc_call* call_ptr_ = nullptr;
  gpr_timespec deadline_;
  grpc_metadata_array initial_metadata_array_;
  grpc_byte_buffer* payload_ptr_ = nullptr;
};

}  // namespace core

#endif  // CORE_SERVER_SERVER_METHOD_CALL_CQTAG_H
