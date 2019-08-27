#include "../../core/common/completion_queue_for_next.h"

#include <grpc/grpc.h>

#include "../../core/common/support/time.h"

namespace core {

CompletionQueueForNext::CompletionQueueForNext()
    : CompletionQueue(grpc_completion_queue_create_for_next(nullptr)) {
}

CompletionQueueForNext::CompletionQueueForNext(grpc_completion_queue* take)
    : CompletionQueue(take) {
}

CompletionQueueForNext::~CompletionQueueForNext() {
}

grpc_event CompletionQueueForNext::NextInternal(gpr_timespec deadline) {
  return grpc_completion_queue_next(&c_cq(), deadline, nullptr);
}

}  // namespace core
