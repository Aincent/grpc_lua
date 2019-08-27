#include "BindClientSyncWriter.h"

#include "../common/GetTimeoutMs.h"

#include "../../core/client/client_sync_writer.h"  // for ClientSyncWriter
#include "../../core/common/status.h"  // for GetCode()

#include <LuaIntf/LuaIntf.h>

#include <string>

using namespace core;
using namespace LuaIntf;

namespace {

ClientSyncWriter GetClientSyncWriter(const ChannelSptr& pChannel,
    const std::string& sMethod, const LuaRef& timeoutSec)
{
    int64_t nTimeoutMs = util::GetTimeoutMs(timeoutSec);
    return ClientSyncWriter(pChannel, sMethod, nTimeoutMs);
}

std::tuple<LuaRef, std::string, grpc_status_code>
Close(const ClientSyncWriter& writer, lua_State* L)
{
    assert(L);
    std::string response;
    Status status = writer.Close(&response);
    LuaRef result = LuaRef(L, nullptr);
    if (status.ok()) result = LuaRef::fromValue(L, response);
    return std::make_tuple(result, status.GetDetails(), status.GetCode());
}  // Close()

}  // namespace

namespace client {

void BindClientSyncWriter(const LuaRef& mod)
{
    lua_State* L = mod.state();
    assert(L);
    LuaBinding(mod).beginClass<ClientSyncWriter>("ClientSyncWriter")
        .addFactory(&GetClientSyncWriter)
        .addFunction("write", &ClientSyncWriter::Write)
        .addFunction("close",
            [L](const ClientSyncWriter* pWriter) {
                assert(pWriter);
                return Close(*pWriter, L);
            })
    .endClass();
}  // BindClientSyncWriter()

}  // namespace client
