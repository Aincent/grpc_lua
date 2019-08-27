#include "BindClientSyncReader.h"

#include "../common/GetTimeoutMs.h"

#include "../../core/client/client_sync_reader.h"  // for ClientSyncReader
#include <LuaIntf/LuaIntf.h>

#include <string>

using namespace core;
using namespace LuaIntf;

namespace {

ClientSyncReader GetClientSyncReader(const ChannelSptr& pChannel,
    const std::string& sMethod, const std::string& sRequest,
    const LuaRef& timeoutSec)
{
    int64_t nTimeoutMs = util::GetTimeoutMs(timeoutSec);
    return ClientSyncReader(pChannel, sMethod, sRequest, nTimeoutMs);
}

// return string|nil, nil means error or end
LuaRef ReadOne(const ClientSyncReader& reader, lua_State* L)
{
    assert(L);
    std::string sMsg;
    if (reader.ReadOne(&sMsg))
        return LuaRef::fromValue(L, sMsg);
    return LuaRef(L, nullptr);
}

}  // namespace

namespace client {

void BindClientSyncReader(const LuaRef& mod)
{
    lua_State* L = mod.state();
    assert(L);
    LuaBinding(mod).beginClass<ClientSyncReader>("ClientSyncReader")
        .addFactory(&GetClientSyncReader)
        .addFunction("read_one",
            [L](const ClientSyncReader* pReader) {
                assert(pReader);
                return ReadOne(*pReader, L);
            })
    .endClass();
}  // BindClientSyncReader()

}  // namespace client
