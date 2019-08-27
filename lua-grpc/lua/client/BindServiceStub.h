#ifndef LUA_CLIENT_BINDSERVICESTUB_H
#define LUA_CLIENT_BINDSERVICESTUB_H

#include "../common/LuaRefFwd.h"  // forward LuaRef

namespace client {
void BindServiceStub(const LuaIntf::LuaRef& mod);
}

#endif  // LUA_CLIENT_BINDSERVICESTUB_H
