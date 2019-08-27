#ifndef LUA_CLIENT_BINDCLIENTSYNCREADER_H
#define LUA_CLIENT_BINDCLIENTSYNCREADER_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindClientSyncReader(const LuaIntf::LuaRef& mod);
}  // namespace client

#endif  // LUA_CLIENT_BINDCLIENTSYNCREADER_H
