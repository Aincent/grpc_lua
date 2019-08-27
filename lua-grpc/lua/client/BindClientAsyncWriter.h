#ifndef LUA_CLIENT_BINDCLIENTASYNCWRITER_H
#define LUA_CLIENT_BINDCLIENTASYNCWRITER_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindClientAsyncWriter(const LuaIntf::LuaRef& mod);
}  // namespace client

#endif  // LUA_CLIENT_BINDCLIENTASYNCWRITER_H
