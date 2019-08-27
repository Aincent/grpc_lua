// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_CLOSE_CB_H
#define CORE_CLIENT_CLOSE_CB_H

#include <functional>
#include <string>

namespace core {

class Status;

// Callback on closed.
using CloseCb = std::function<void (const Status&, const std::string&)>;

}  // namespace core
#endif  // CORE_CLIENT_CLOSE_CB_H
