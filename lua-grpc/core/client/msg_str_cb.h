// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CLIENT_MSG_STR_CB_H
#define CORE_CLIENT_MSG_STR_CB_H

#include <functional>
#include <string>

namespace core {

class Status;

// Message string callback function.
using MsgStrCb = std::function<Status (const std::string& message)>;
// Response string callback function.
using RespStrCb = MsgStrCb;

}  // namespace core
#endif  // CORE_CLIENT_MSG_STR_CB_H
