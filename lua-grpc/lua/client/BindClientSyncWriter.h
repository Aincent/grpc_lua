#ifndef LUA_CLIENT_BINDCLIENTSYNCWRITER_H
#define LUA_CLIENT_BINDCLIENTSYNCWRITER_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindClientSyncWriter(const LuaIntf::LuaRef& mod);
}  // namespace client

#endif  // LUA_CLIENT_BINDCLIENTSYNCWRITER_H
