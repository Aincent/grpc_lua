/*
 * grpc_lua.cpp
 *
 *  Created on: Jul 23, 2019
 *      Author: root
 */

//pb
#include "pb/BindPb.h"
//Net
#include "net/BindNet.h"
// client
#include "lua/client/BindChannel.h"
#include "lua/client/BindClientAsyncReaderWriter.h"
#include "lua/client/BindClientAsyncWriter.h"
#include "lua/client/BindClientSyncReader.h"
#include "lua/client/BindClientSyncReaderWriter.h"
#include "lua/client/BindClientSyncWriter.h"
#include "lua/client/BindServiceStub.h"

// server
#include "lua/server/BindServer.h"
#include "lua/server/BindServerReplier.h"
#include "lua/server/BindServerWriter.h"

#include <LuaIntf/LuaIntf.h>

extern "C"
{
int luaopen_grpc(lua_State* L)
{
    assert(L);
    LuaIntf::LuaRef mod = LuaIntf::LuaRef::createTable(L);
    // client
    client::BindChannel(mod);
    client::BindClientAsyncReaderWriter(mod);
    client::BindClientAsyncWriter(mod);
    client::BindClientSyncReader(mod);
    client::BindClientSyncReaderWriter(mod);
    client::BindClientSyncWriter(mod);
    client::BindServiceStub(mod);

    // server
    server::BindServer(mod);
    server::BindServerReplier(mod);
    server::BindServerWriter(mod);

    //LuaPbIntfImpl
    pb::BindPb(mod);

    //net
    net::BindNet(mod);

    mod.pushToStack();

    return 1;
}  // luaopen_grpc_lua()
}



