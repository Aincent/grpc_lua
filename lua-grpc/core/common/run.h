#ifndef CORE_RUN_H
#define CORE_RUN_H

#include "../../core/common/completion_queue_for_next_sptr.h"  // for CompletionQueueForNextSptr
#include "../../core/common/support/core_api.h"  // for CORE_API

namespace core {

CORE_API void Run(CompletionQueueForNext& cq4n);
CORE_API void UnBlockingRun(CompletionQueueForNext& cq4n);

CORE_API void UnBlockingRun(const CompletionQueueForNextSptr& cq4n_sptr);
CORE_API void Run(const CompletionQueueForNextSptr& cq4n_sptr);

}  // namespace core
#endif  // CORE_RUN_H
