#ifndef LUA_CLIENT_BINDCHANNEL_H
#define LUA_CLIENT_BINDCHANNEL_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindChannel(const LuaIntf::LuaRef& mod);
}  // namespace client

#endif  // LUA_CLIENT_BINDCHANNEL_H
