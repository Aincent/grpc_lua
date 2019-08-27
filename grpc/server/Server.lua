local c = require("grpc")  -- from grpc.so
local Service = require "grpc/server/Service"

local Server = {}
--- New server.
-- @treturn table `Server` object
function Server:new()
    local svr = {
        c_svr = c.Server(),
    }
    setmetatable(svr, self)
    self.__index = self
    return svr
end
----------------------------------Server -------------------------------------------------
--- Add listening host port pair.
-- @string host_port host and port string, like: "0.0.0.0:50051"
-- @treturn int bound port number on success, 0 on failure.
-- @usage svr:add_listening_port("0.0.0.0:50051")
function Server:add_listening_port(host_port)
    -- assert("string" == type(host_port))
    self.c_svr:add_listening_port(host_port)
end

function Server:register_service(service_name, service)
    -- assert("string" == type(service_name))
    -- assert("table" == type(service))
    local desc = c.get_service_descriptor(service_name)
    -- assert("table" == type(desc))  -- a service descriptor proto message

    -- service impl table is wrapped by lua Service,
    --  which is then wrapped by C Service, and then is registered in server.
    self.c_svr:register_service(Service:new(service_name, desc, service))
end

--- UnBlocking run the server.
function Server:run()
    self.c_svr:run()
end

return Server