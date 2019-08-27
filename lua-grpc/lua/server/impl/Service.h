/*
 * Service.h
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

#ifndef LUA_SERVER_IMPL_SERVICE_H_
#define LUA_SERVER_IMPL_SERVICE_H_

#include "../../common/LuaRefFwd.h"  // forward LuaRef
#include "../../../core/server/service.h"  // for Service and CallSptr

#include <vector>

namespace LuaIntf {
	struct LuaString;
}  // namespace LuaIntf

namespace impl {

// Adapt lua service table to grpc_cb_core::Service.
class Service : public core::Service
{
public:
    using LuaRef = LuaIntf::LuaRef;
    explicit Service(const LuaRef& luaService);
    ~Service();

public:
    using string = std::string;
    const string& GetFullName() const override;
    size_t GetMethodCount() const override;
    bool IsMethodClientStreaming(size_t iMthdIdx) const override;
    const string& GetMethodName(size_t iMthdIdx) const override;
    void CallMethod(size_t iMthdIdx, grpc_byte_buffer* pReqBuf,
        const core::CallSptr& pCall) override;

private:
    void InitMethods();
    void InitMethod(size_t iMthdIdx, const LuaRef& luaMethod);
    void CallClientStreamingMethod(size_t iMthdIdx,
        const core::CallSptr& pCall);
    void CallNonClientStreamingMethod(size_t iMthdIdx,
        LuaIntf::LuaString& strReq, const core::CallSptr& pCall);

private:
    std::unique_ptr<const LuaRef> m_pLuaService;
    string m_sFullName;  // like "helloworld.Greeter"

    struct MethodInfo
    {
        // string sName;  like "SayHello"
        // Method request name is like: "/helloworld.Greeter/SayHello"
        string sRequestName;
        bool IsClientStreaming;
        bool IsServerStreaming;
    };  // struct MethodInfo
    std::vector<MethodInfo> m_vMethods;
};  // class Service

}  // namespace impl

#endif /* LUA_SERVER_IMPL_SERVICE_H_ */
