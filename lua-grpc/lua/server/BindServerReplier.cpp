#include "BindServerReplier.h"

#include "../../core/common/status.h"  // for Status
#include "../../core/server/server_replier.h"  // for ServerReplier
#include <LuaIntf/LuaIntf.h>
#include <string>

using namespace core;
using namespace LuaIntf;

static void ReplyError(ServerReplier* pReplier, const std::string& sError, int nStatusCode)
{
    assert(pReplier);
    pReplier->ReplyError(Status(static_cast<grpc_status_code>(nStatusCode), sError));
}

namespace server {

void BindServerReplier(const LuaRef& mod)
{
    LuaBinding(mod).beginClass<ServerReplier>("ServerReplier")
        .addConstructor(LUA_ARGS(const core::CallSptr&))
        .addFunction("reply", &ServerReplier::ReplyStr)
        .addFunction("reply_error", &ReplyError)
    .endClass();
}  // BindServerReplier()

}  // namespace server
