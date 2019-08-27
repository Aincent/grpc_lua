// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_COMPLETION_QUEUE_SPTR_H
#define CORE_COMPLETION_QUEUE_SPTR_H

#include <memory>

namespace core {
class CompletionQueue;
using CompletionQueueSptr = std::shared_ptr<CompletionQueue>;
}  // namespace core

#endif  // CORE_COMPLETION_QUEUE_SPTR_H
