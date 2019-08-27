#include "BindServerWriter.h"

#include "../../core/common/status.h"  // for Status
#include "../../core/server/server_writer.h"  // for ServerWriter

#include <LuaIntf/LuaIntf.h>
#include <string>

using namespace core;
using namespace LuaIntf;

namespace {

void Close(const ServerWriter* pWriter)
{
    assert(pWriter);
    pWriter->AsyncClose(Status::OK);
}  // Close()

}  // namespace

namespace server {

void BindServerWriter(const LuaRef& mod)
{
    LuaBinding(mod).beginClass<ServerWriter>("ServerWriter")
        .addConstructor(LUA_ARGS(const core::CallSptr&))
        .addFunction("write", &ServerWriter::Write)
        .addFunction("close", &Close)
    .endClass();
}  // BindServerWriter()

}  // namespace server
