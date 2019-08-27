// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_STATUS_CB_H
#define CORE_CLIENT_STATUS_CB_H

#include <functional>  // for std::function()

namespace core {

class Status;
using StatusCb = std::function<void (const Status& status)>;
using ErrorCb = StatusCb;

}  // namespace core
#endif  // CORE_CLIENT_STATUS_CB_H
