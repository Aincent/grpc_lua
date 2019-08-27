local skynet    = require "skynet.manager"
local c = require("grpc")  -- from grpc.so
local Server = require "grpc/server/Server"
local ServiceStub = require "grpc/client/ServiceStub"

local TAG = "grpc"

local grpc = {}
-------------------------------------Server ---------------------------------------------
function grpc.server()
    return  Server:new()
end
-------------------------------------Client ---------------------------------------------
--- Create a channel.
-- @string host_port input "host:port" like: "a.b.com:6666" or "1.2.3.4:6666"
-- @return C `Channel` object
-- @usage ch = grpc.channel("localhost:50051")
function grpc.channel(host_port)
    -- assert("string" == type(host_port))
    return c.Channel(host_port)
end

--- Create service stub.
-- @tparam userdata|string channel C `Channel` object, from `grpc_lua.channel()`,
-- or a string of "host:port" like "a.b.com:6666" or "1.2.3.4:6666"
-- @string[opt] service_name service name, like "helloworld.Greeter"
-- @treturn ServiceStub service stub object
-- @usage stub = grpc_lua.ServiceStub(grpc_lua.channel("localhost:50051"))
-- @usage stub = grpc_lua.ServiceStub("localhost:50051", "helloworld.Greeter")
function grpc.service_stub(channel, service_name)
    if "string" == type(channel) then
        channel = grpc.channel(channel)
    end
    -- assert("userdata" == type(channel))
    return ServiceStub:new(channel, service_name)
end
-----------------------------------protoc---------------------------------------------------
--- Add proto path as `protoc --proto_path=PATH`.
-- Specify the directory in which to search for imports.
-- May add multiple times to set multiple proto paths,
-- and the directories will be searched in order.
-- The current working directory is always added first automatically.
-- Directly calls `luaphintf.add_proto_path()`
-- @string proto_path the directory name
function grpc.add_proto_path(proto_path)
    c.add_proto_path(proto_path)
end  -- add_proto_path()

--- Map a path on disk to a location in the SourceTree.
-- See Protobuf `DiskSourceTree::MapPath()`.
-- @string virtual_path virtual path
-- @string disk_path disk path
function grpc.map_path(virtual_path, disk_path)
    c.map_path(virtual_path, disk_path)
end  -- map_path()

--- Import the given file to a FileDescriptor.
-- Dependencies are imported automatically.
-- @string file_name *.proto file name, must be relative to proto path.
-- @usage import_proto_file("foo.proto")
function grpc.import_proto_file(file_name)
    c.import_proto_file(file_name)
end

function grpc.get_intranet_ip()
    return c.get_intranet_ip()
end

return grpc