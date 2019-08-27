#ifndef LUA_CLIENT_IMPL_CBWRAPPER_H
#define LUA_CLIENT_IMPL_CBWRAPPER_H

#include "../../common/LuaRefFwd.h"  // forward LuaRef

#include "../../../core/client/msg_str_cb.h"  // for MsgStrCb
#include "../../../core/client/status_cb.h"  // for StatusCb
#include "../../../core/client/msg_bool_int_cb.h" // for WFSCb

namespace CbWrapper {

// Convert lua message callback into MsgStrCb.
// function(string):(string|nil) | nil -> Status (const string&)
core::MsgStrCb WrapLuaMsgCb(const LuaIntf::LuaRef& luaMsgCb);

// Convert lua status callback into StatusCb.
// function(error_str, status_code) -> void (const Status&)
core::StatusCb WrapLuaStatusCb(const LuaIntf::LuaRef& luaStatusCb);

core::StatusChangeCb WrapLuaWaitForStatusCb(const LuaIntf::LuaRef& luaWaitForStatusCb);

// Convert lua error callback into ErrorCb.
// function(error_str, status_code) -> void (const Status&)
inline core::ErrorCb WrapLuaErrorCb(const LuaIntf::LuaRef& luaErrorCb)
{
    return WrapLuaStatusCb(luaErrorCb);
}

}  // namespace CbWrapper

#endif  // LUA_CLIENT_IMPL_CBWRAPPER_H
