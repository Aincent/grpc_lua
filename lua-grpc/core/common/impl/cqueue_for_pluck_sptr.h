// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_IMPL_CQUEUE_FOR_PLUCK_SPTR_H
#define CORE_IMPL_CQUEUE_FOR_PLUCK_SPTR_H

#include <memory>

namespace core {
class CQueueForPluck;
using CQueueForPluckSptr = std::shared_ptr<CQueueForPluck>;
}  // namespace core

#endif  // CORE_IMPL_CQUEUE_FOR_PLUCK_SPTR_H
