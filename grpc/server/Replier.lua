--- Wraps C `ServerReplier` class.
-- @classmod grpc.core.server.ServerReplier
local c = require("grpc")  -- from grpc.so
local Replier = {}

-------------------------------------------------------------------------------
--- Public functions.
-- @section public

--- New Replier.
-- Used by `Service`. No not call it directly.
-- @tparam userdata c_replier C Replier object
-- @string response_type response type, like "helloworld.HelloResponse"
-- @treturn table Replier object
function Replier:new(c_replier, response_type)
    -- assert("userdata" == type(c_replier))
    -- assert("string" == type(response_type))

    local replier = {
        -- private:
        _c_replier = c_replier,
        _response_type = response_type,
    }

    setmetatable(replier, self)
    self.__index = self
    return replier
end

--- Reply response.
-- @tab response response
function Replier:reply(response)
    -- assert("table" == type(response))
    local resp_str = c.encode(self._response_type, response)
    self._c_replier:reply(resp_str)
end  

--- Reply error.
-- @string error_str
-- @int status_code
function Replier:reply_error(error_str, status_code)
    -- assert("string" == type(error_str))
    -- assert("number" == type(status_code))
    self._c_replier:reply_error(error_str, status_code)
end

return Replier
