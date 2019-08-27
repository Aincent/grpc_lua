/*
 * Service.cpp
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

#include "Service.h"

#include "ServerReader.h"  // for ServerReader

#include <grpc/byte_buffer.h>  // for grpc_byte_buffer_reader_init()
#include <grpc/byte_buffer_reader.h>  // for grpc_byte_buffer_reader
#include "../../../core/server/server_replier.h"  // for ServerReplier
#include "../../../core/server/server_writer.h"  // for ServerWriter
#include <LuaIntf/LuaIntf.h>

#include <cassert>
#include <sstream>  // for ostringstream

using namespace core;

namespace impl {

Service::Service(const LuaRef& luaService) : m_pLuaService(new LuaRef(luaService))
{
    luaService.checkTable();

    m_sFullName = luaService.dispatch<std::string>("get_full_name");

    InitMethods();
}

Service::~Service()
{

}

const std::string& Service::GetFullName() const
{
    return m_sFullName;
}

size_t Service::GetMethodCount() const
{
    return m_vMethods.size();
}

bool Service::IsMethodClientStreaming(size_t iMthdIdx) const
{
    assert(iMthdIdx < GetMethodCount());  // XXX is assert enough?
    return m_vMethods[iMthdIdx].IsClientStreaming;
}

// Return method request name, like: "/helloworld.Greeter/SayHello"
// Not full_name: "helloworld.Greeter.SayHello"
// Used by service register.
const std::string& Service::GetMethodName(size_t iMthdIdx) const
{
    assert(iMthdIdx < GetMethodCount());
    return m_vMethods[iMthdIdx].sRequestName;
}

static grpc_slice BufToSlice(grpc_byte_buffer* buf)
{
    grpc_byte_buffer_reader bbr;
    grpc_byte_buffer_reader_init(&bbr, buf);
    grpc_slice slice = grpc_byte_buffer_reader_readall(&bbr);
    grpc_byte_buffer_reader_destroy(&bbr);
    return slice;
}

void Service::CallMethod(size_t iMthdIdx, grpc_byte_buffer* pReqBuf, const CallSptr& pCall)
{
    assert(iMthdIdx < GetMethodCount());
    if (!pReqBuf)
    {
        CallClientStreamingMethod(iMthdIdx, pCall);
        return;
    }

    grpc_slice req_slice = BufToSlice(pReqBuf);
    {
        LuaIntf::LuaString strReq(reinterpret_cast<const char*>(
            GRPC_SLICE_START_PTR(req_slice)),
            GRPC_SLICE_LENGTH(req_slice));
        CallNonClientStreamingMethod(iMthdIdx, strReq, pCall);
    }
    grpc_slice_unref(req_slice);
}

void Service::InitMethods()
{
    assert(m_vMethods.empty());

    LuaRef luaSvcDesc = m_pLuaService->dispatch<LuaRef>("get_descriptor");
    luaSvcDesc.checkTable();
    // See doc/service_descriptor_example.txt
    LuaRef luaMethods = luaSvcDesc.get("method");
    luaMethods.checkTable();

    int len = luaMethods.len();
    m_vMethods.resize(len);
    for (int i = 0; i < len; ++i)
    {
        LuaRef luaMethod = luaMethods[i + 1];
        luaMethod.checkTable();
        InitMethod(i, luaMethod);
    }
}

// ".test.CommonMsg" -> "test.CommonMsg"
static std::string TrimLeadingDot(const std::string& s)
{
    if (s.empty()) return s;
    if ('.' != s[0]) return s;
    return s.substr(1);
}

void Service::InitMethod(size_t iMthdIdx, const LuaRef& luaMethod)
{
    assert(luaMethod.isTable());
    MethodInfo& r = m_vMethods[iMthdIdx];
    std::string sName = luaMethod.get<string>("name");
    // Method name is like: "/helloworld.Greeter/SayHello"
    r.sRequestName = "/" + m_sFullName + "/" + sName;
    r.IsClientStreaming = luaMethod.get<bool>("client_streaming");
    r.IsServerStreaming = luaMethod.get<bool>("server_streaming");
}

void Service::CallClientStreamingMethod(size_t iMthdIdx, const CallSptr& pCall)
{
    assert(iMthdIdx < GetMethodCount());
    const MethodInfo& mi = m_vMethods[iMthdIdx];
    if (!mi.IsClientStreaming) return;

    assert(m_pLuaService->isTable());
    LuaRef luaReader;
    if (mi.IsServerStreaming)
    {
        luaReader = m_pLuaService->dispatch<LuaRef>(
            "call_bidi_streaming_method", iMthdIdx+1, ServerWriter(pCall));
    }
    else
    {
        luaReader = m_pLuaService->dispatch<LuaRef>(
            "call_c2s_streaming_method", iMthdIdx+1, ServerReplier(pCall));
    }
    std::make_shared<ServerReader>(luaReader)->Start(pCall);
}

void Service::CallNonClientStreamingMethod(size_t iMthdIdx,
    LuaIntf::LuaString& strReq, const CallSptr& pCall)
{
    assert(iMthdIdx < GetMethodCount());
    const MethodInfo& mi = m_vMethods[iMthdIdx];
    if (mi.IsClientStreaming) return;

    assert(m_pLuaService->isTable());
    if (mi.IsServerStreaming)
    {
        m_pLuaService->dispatch("call_s2c_streaming_method",
            iMthdIdx+1, strReq, ServerWriter(pCall));
        return;
    }

    m_pLuaService->dispatch("call_simple_method",
        iMthdIdx+1, strReq, ServerReplier(pCall));
}

}  // namespace impl



