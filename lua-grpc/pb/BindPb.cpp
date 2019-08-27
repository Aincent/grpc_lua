/*
 * BindPb.cpp
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */
#include "BindPb.h"
#include <LuaIntf/LuaIntf.h>
#include <string>
#include "LuaPbIntfImpl.h"


using namespace LuaIntf;

namespace pb
{
//	void BindPb(const LuaIntf::LuaRef& mod) {
//	    LuaBinding(mod).beginClass<LuaPbIntfImpl>("LuaPb").addConstructor(LUA_ARGS())
//	        // Returns bound port number on success, 0 on failure.
//	        .addFunction("add_proto_path", &LuaPbIntfImpl::AddProtoPath)
//	        .addFunction("map_path", &LuaPbIntfImpl::MapPath)
//	        .addFunction("import_proto_file", &LuaPbIntfImpl::ImportProtoFile)
//			.addFunction("encode", &LuaPbIntfImpl::Encode)
//			.addFunction("decode", &LuaPbIntfImpl::Decode)
//			.addFunction("get_service_descriptor", &LuaPbIntfImpl::GetServiceDescriptorTbl)
//			.addFunction("get_rpc_input_name", &LuaPbIntfImpl::GetRpcInputName)
//			.addFunction("get_rpc_output_name", &LuaPbIntfImpl::GetRpcOutputName)
//			.addFunction("is_rpc_client_streaming", &LuaPbIntfImpl::IsRpcClientStreaming)
//			.addFunction("is_rpc_server_streaming", &LuaPbIntfImpl::IsRpcServerStreaming)
//	    .endClass();
//	}


extern "C"
{
	void AddProtoPath(lua_State* L, const std::string& sProtoPath) {
		LuaPbIntfImpl::getInstance()->AddProtoPath(sProtoPath);
	}

	void MapPath(lua_State* L, const std::string& sVirtualPath, const std::string& sDiskPath) {
		LuaPbIntfImpl::getInstance()->MapPath(sVirtualPath, sDiskPath);
	}

	void ImportProtoFile(lua_State* L, const std::string& sProtoFile) {
		LuaPbIntfImpl::getInstance()->ImportProtoFile(sProtoFile);
	}

    std::string Encode(lua_State* L, const std::string& sMsgTypeName, const LuaRef& luaTable){
    	return LuaPbIntfImpl::getInstance()->Encode(sMsgTypeName, luaTable);
    }

    LuaIntf::LuaRef Decode(lua_State* L, const std::string& sMsgTypeName, const std::string& sData){
    	return LuaPbIntfImpl::getInstance()->Decode(L, sMsgTypeName, sData);
    }

	LuaIntf::LuaRef GetServiceDescriptorTbl(lua_State* L, const std::string& sServiceName)
	{
		return LuaPbIntfImpl::getInstance()->GetServiceDescriptorTbl(L, sServiceName);
	}

	std::string GetRpcInputName(lua_State* L, const std::string& sServiceName, const std::string& sMethodName){
		return LuaPbIntfImpl::getInstance()->GetRpcInputName(sServiceName, sMethodName);
	}

	std::string GetRpcOutputName(lua_State* L, const std::string& sServiceName, const std::string& sMethodName){
		return LuaPbIntfImpl::getInstance()->GetRpcOutputName(sServiceName, sMethodName);
	}

    bool IsRpcClientStreaming(lua_State* L, const std::string& sServiceName, const std::string& sMethodName){
    	return LuaPbIntfImpl::getInstance()->IsRpcClientStreaming(sServiceName, sMethodName);
    }

    bool IsRpcServerStreaming(lua_State* L, const std::string& sServiceName, const std::string& sMethodName){
    	return LuaPbIntfImpl::getInstance()->IsRpcServerStreaming(sServiceName, sMethodName);
    }

	void BindPb(const LuaIntf::LuaRef& mod) {
	    LuaIntf::LuaBinding(mod)
	        .addFunction("add_proto_path", &AddProtoPath)

	        .addFunction("map_path", &MapPath)

	        .addFunction("import_proto_file", &ImportProtoFile)

	        .addFunction("encode", &Encode)

	        .addFunction("decode", &Decode)

		    .addFunction("get_service_descriptor", &GetServiceDescriptorTbl)

	        .addFunction("get_rpc_input_name", &GetRpcInputName)

	        .addFunction("get_rpc_output_name", &GetRpcOutputName)

	        .addFunction("is_rpc_client_streaming",&IsRpcClientStreaming)

	        .addFunction("is_rpc_server_streaming", &IsRpcServerStreaming)
	        ;  // LuaBinding(mod)
	}
}
}
