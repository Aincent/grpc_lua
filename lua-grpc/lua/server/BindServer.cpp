/*
 * BindServer.cpp
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

#include "BindServer.h"

#include "impl/Service.h"  // for Service

#include "../../core/server/server.h"  // for Server
#include <LuaIntf/LuaIntf.h>
#include <string>

using namespace LuaIntf;


namespace {
void RegisterService(core::Server* pServer, const LuaRef& luaService)
{
    assert(pServer);
    luaService.checkTable();
    pServer->RegisterService(std::make_shared<impl::Service>(luaService));
}
}

namespace server {

void BindServer(const LuaRef& mod)
{
    using namespace core;
    LuaBinding(mod).beginClass<Server>("Server").addConstructor(LUA_ARGS())
        // Returns bound port number on success, 0 on failure.
        .addFunction("add_listening_port", static_cast<int(Server::*)(const std::string&)>(&Server::AddListeningPort))
        .addFunction("register_service", &RegisterService)
        .addFunction("run", &Server::Run)
    .endClass();  // Server
}  // BindServer()

}  // namespace server



