// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_COMMON_IMPL_COMPLETE_CB_H
#define CORE_COMMON_IMPL_COMPLETE_CB_H

#include <functional>  // for function<>

namespace core {

// Callback on completion
using CompleteCb = std::function<void (bool success)>;

}  // namespace core
#endif  // CORE_COMMON_IMPL_COMPLETE_CB_H
