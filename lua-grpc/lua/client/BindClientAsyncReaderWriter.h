#ifndef LUA_CLIENT_BINDCLIENTASYNCREADERWRITER_H
#define LUA_CLIENT_BINDCLIENTASYNCREADERWRITER_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindClientAsyncReaderWriter(const LuaIntf::LuaRef& mod);
}  // namespace client

#endif  // LUA_CLIENT_BINDCLIENTASYNCREADERWRITER_H
