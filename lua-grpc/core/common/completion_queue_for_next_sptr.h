// Shared pointer to complete queue for next.
#ifndef CORE_COMPLETION_QUEUE_FOR_NEXT_SPTR_H
#define CORE_COMPLETION_QUEUE_FOR_NEXT_SPTR_H

#include <memory>

namespace core {

class CompletionQueueForNext;
using CompletionQueueForNextSptr = std::shared_ptr<CompletionQueueForNext>;

}  // namespace core
#endif  // CORE_COMPLETION_QUEUE_FOR_NEXT_SPTR_H
