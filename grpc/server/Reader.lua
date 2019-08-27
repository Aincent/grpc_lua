--- Server reader class to wrap user's reader implemention.
-- It keeps the message type to decode message string into message table.
-- @classmod grpc.core.server.ServerReader
local c = require("grpc")  -- from grpc.so
local Reader = {}
-------------------------------------------------------------------------------
--- Public functions.
-- @section public

--- New Reader.
-- @table impl user implemented reader object with `on_msg()` function,
--  which will be adapted to `on_msg_str()` function.
-- @string msg_type message type, like "routeguide.RouteNote"
-- @treturn table Reader object
function Reader:new(pb, impl, msg_type)
    -- assert("table" == type(impl))
    -- assert("string" == type(msg_type))
    local reader = {
        -- private:
        _impl = impl,
        _msg_type = msg_type,
    }

    setmetatable(reader, self)
    self.__index = self
    return reader
end

--- Message string handler.
-- @string msg_str
-- @treturn string|nil error string, nil if no error
function Reader:on_msg_str(msg_str)
    if not self._impl.on_msg then return nil end
    local msg = c.decode(self._msg_type, msg_str)
    if msg then
        self._impl:on_msg(msg)
        return nil
    end
    return "Failed to decode message " .. self._msg_type
end

--- Error handler.
-- @string error_str
-- @int status_code
function Reader:on_error(error_str, status_code)
    if not self._impl.on_error then return end
    self._impl:on_error(error_str, status_code)
end

--- End handler.
function Reader:on_end()
    if not self._impl.on_end then return end
    self._impl:on_end()
end

-------------------------------------------------------------------------------
--- Private functions.
-- @section private

return Reader
