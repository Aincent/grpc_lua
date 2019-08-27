// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_COMMON_DO_NEXT_COMPLETION_H
#define CORE_COMMON_DO_NEXT_COMPLETION_H

namespace core {

class CompletionQueueForNext;

// Blocking go next and complete.
// Return false if completion queue is shut down.
bool DoNextCompletion(CompletionQueueForNext& cq4n);

// UnBlocking go next and complete.
// Return false if completion queue is shut down.
bool DoTryNextCompletion(CompletionQueueForNext& cq4n);

}  // namespace grpv_cb
#endif  // CORE_COMMON_DO_NEXT_COMPLETION_H
