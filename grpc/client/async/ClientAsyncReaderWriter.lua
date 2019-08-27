--- Client async reader writer class.
-- @classmod grpc_lua.client.async.ClientAsyncReaderWriter
local c = require("grpc")  -- from grpc.so
local mcb_wrapper = require("grpc/client/msg_cb_wrapper")

local ClientAsyncReaderWriter = {}
-------------------------------------------------------------------------------
--- Public functions.
-- @section public

--- Constructor.
-- @param c_stub c.ServiceStub object
-- @string request_name, like "/routeguide.RouteGuide/RouteChat"
-- @string request_type
-- @string response_type
-- @tparam number|nil timeout_sec nil means no timeout
-- @tparam function|nil status_cb `function(error_str, status_code)`
-- @treturn table object
function ClientAsyncReaderWriter:new(c_stub, request_name, request_type, response_type, timeout_sec, status_cb)
    -- assert("userdata" == type(c_stub))
    -- assert("string" == type(request_name))
    -- assert("string" == type(request_type))
    -- assert("string" == type(response_type))
    -- assert(not timeout_sec or "number" == type(timeout_sec))
    -- assert(not status_cb or "function" == type(status_cb))
    local rdwr = {
        -- private:
        _c_rdwr = c.ClientAsyncReaderWriter(c_stub,
            request_name, timeout_sec, status_cb),
        _request_type = request_type,
        _response_type = response_type,
    }

    setmetatable(rdwr, self)
    self.__index = self
    return rdwr
end  -- new()

--- Read each message.
-- @tparam function|nil msg_cb message callback, nil to ignore all messages
-- `function(table)`
-- @treturn table|nil message table, nil means error or end
function ClientAsyncReaderWriter:read_each(msg_cb)
    local msg_str_cb = nil  -- nil|function(string)
    if msg_cb then
        -- assert("function" == type(msg_cb))
        msg_str_cb = mcb_wrapper.wrap(msg_cb, self._response_type)
    end
    self._c_rdwr:read_each(msg_str_cb)
end  -- read_one()

--- Write message.
-- @table message
-- @treturn boolean return false on error
function ClientAsyncReaderWriter:write(message)
    -- assert("table" == type(message))
    local msg_str = c.encode(self._request_type, message)
    self._c_rdwr:write(msg_str)
end

--- Close writing.
-- Optional. Writing is auto closed in dtr().
function ClientAsyncReaderWriter:close_writing()
    self._c_rdwr:close_writing()
end

-------------------------------------------------------------------------------
--- Private functions.
-- @section private

return ClientAsyncReaderWriter
