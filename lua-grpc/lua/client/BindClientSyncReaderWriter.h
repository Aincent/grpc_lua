#ifndef LUA_CLIENT_BINDCLIENTSYNCREADERWRITER_H
#define LUA_CLIENT_BINDCLIENTSYNCREADERWRITER_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindClientSyncReaderWriter(const LuaIntf::LuaRef& mod);
}  // namespace client

#endif  // LUA_CLIENT_BINDCLIENTSYNCREADERWRITER_H
