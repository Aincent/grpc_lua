// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_COMMON_CALL_SPTR_H
#define CORE_COMMON_CALL_SPTR_H

#include <memory>

namespace core {
class Call;
using CallSptr = std::shared_ptr<Call>;
}  // namespace core

#endif  // CORE_COMMON_CALL_SPTR_H
