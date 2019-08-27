// Licensed under the Apache License, Version 2.0.
// Author: Jin Qing (http://blog.csdn.net/jq0123)

#ifndef CORE_CHANNEL_SPTR_H
#define CORE_CHANNEL_SPTR_H

#include <memory>  // for shared_ptr

namespace core {
class Channel;
using ChannelSptr = std::shared_ptr<Channel>;
}  // namespace core

#endif  // CORE_CHANNEL_SPTR_H
