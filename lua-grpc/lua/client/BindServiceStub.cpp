#include "BindServiceStub.h"

#include "impl/CbWrapper.h"
#include "../common/GetTimeoutMs.h"

#include "../../core/client/channel.h"             // for Channel
#include "../../core/client/client_async_reader.h" // for ClientAsyncReader
#include "../../core/client/service_stub.h"        // for ServiceStub
#include "../../core/common/completion_queue_for_next.h" // to cast GetCompletionQueue()
#include "../../core/common/status.h"                    // for Status

#include <LuaIntf/LuaIntf.h>
#include <string>

using namespace core;
using namespace LuaIntf;
using string = std::string;

namespace {

void SetErrorCb(ServiceStub* pServiceStub, const LuaRef& luaErrorCb)
{
    assert(pServiceStub);
    pServiceStub->SetErrorCb(CbWrapper::WrapLuaErrorCb(luaErrorCb));
}  // SetErrorCb()

void SetTimeoutSec(ServiceStub* pServiceStub, const LuaRef& luaTimeoutSec)
{
    assert(pServiceStub);
    int64_t nTimeoutMs = util::GetTimeoutMs(luaTimeoutSec);
    pServiceStub->SetCallTimeoutMs(nTimeoutMs);
}  // SetTimeoutSec()


void SetWaitForStateChange(ServiceStub* pServiceStub, const LuaRef& luaWaitForStateChangeCb)
{
    assert(pServiceStub);
    StatusChangeCb scCb = CbWrapper::WrapLuaWaitForStatusCb(luaWaitForStateChangeCb);
	pServiceStub->SetStatusChangeCb(scCb);
}

// Sync request.
// Return (response_str|nil, error_str|nil, status_code).
std::tuple<string, string, string>
SyncRequest(ServiceStub& rServiceStub, const string& sMethod,
    const string& sRequest, lua_State* L)
{
    assert(L);
    string sResponse;
    Status status = rServiceStub.SyncRequest(sMethod, sRequest, sResponse);
    if (status.ok())
    {
        // (response_str, nil, status_code)
        return std::make_tuple(sResponse, "", std::to_string(status.GetCode()));
    }
    // (nil, error_str, status_code)
    return std::make_tuple("", status.GetDetails(), std::to_string(status.GetCode()));
}  // SyncRequest()

void AsyncRequest(ServiceStub* pServiceStub,
    const string& sMethod, const string& sRequest,
    const LuaRef& luaResponseCb, const LuaRef& luaErrorCb)
{
    assert(pServiceStub);
    RespStrCb cbRespStr = CbWrapper::WrapLuaMsgCb(luaResponseCb);
    ErrorCb cbError = CbWrapper::WrapLuaErrorCb(luaErrorCb);
    pServiceStub->AsyncRequest(sMethod, sRequest, cbRespStr, cbError);
}  // AsyncRequest()

void AsyncRequestRead(ServiceStub* pServiceStub,
    const string& sMethod, const string sRequest,
    const LuaRef& luaMsgCb, const LuaRef& luaStatusCb)
{
    ClientAsyncReader reader(pServiceStub->GetChannelSptr(),
        sMethod, sRequest, pServiceStub->GetCompletionQueue(),
        pServiceStub->GetCallTimeoutMs());
    reader.ReadEach(CbWrapper::WrapLuaMsgCb(luaMsgCb),
        CbWrapper::WrapLuaErrorCb(luaStatusCb));
}  // AsyncRequestRead()

}  // namespace

namespace client {

void BindServiceStub(const LuaRef& mod)
{
	lua_State* L = mod.state();

    LuaBinding(mod).beginClass<ServiceStub>("ServiceStub")
        .addConstructor(LUA_ARGS(const ChannelSptr& pChannel))

        .addFunction("set_error_cb", &SetErrorCb)
        .addFunction("set_timeout_sec", &SetTimeoutSec)
		.addFunction("get_channel_status", &ServiceStub::GetChannelStatus)
		.addFunction("set_wait_for_state_change_cb", &SetWaitForStateChange)

        .addFunction("sync_request",
            [L](ServiceStub* pServiceStub, const string& sMethod,
                    const string& sRequest) {
                assert(pServiceStub);
                return SyncRequest(*pServiceStub, sMethod, sRequest, L);
            })
        .addFunction("async_request", &AsyncRequest)

        // Async request server side streaming.
        .addFunction("async_request_read", &AsyncRequestRead)

        .addFunction("run", &ServiceStub::Run)
        .addFunction("shutdown", &ServiceStub::Shutdown)
    .endClass();  // ServiceStub
}  // BindServiceStub()

}  // namespace client
