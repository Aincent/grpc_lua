--- Client async writer class.
-- @classmod grp.client.async.ClientAsyncWriter

local c = require("grpc")  -- from grpc.so

local ClientAsyncWriter = {}
-------------------------------------------------------------------------------
--- Public functions.
-- @section public

--- Constructor.
-- @param c_stub c.ServiceStub object
-- @string request_name like "/routeguide.RouteGuide/ListFeatures"
-- @string request_type
-- @string response_type
-- @tparam number|nil timeout_sec nil means no timeout
-- @treturn table object
function ClientAsyncWriter:new(c_stub, request_name, request_type, response_type, timeout_sec)
    -- assert("userdata" == type(c_stub))
    -- assert("string" == type(request_name))
    -- assert("string" == type(request_type))
    -- assert("string" == type(response_type))
    local writer = {
        -- private:
        _c_writer = c.ClientAsyncWriter(c_stub, request_name, timeout_sec),
        _request_type = request_type,
        _response_type = response_type,
    }

    setmetatable(writer, self)
    self.__index = self
    return writer
end  -- new()

--- Write message.
-- @table message
-- @treturn boolean return false on error
function ClientAsyncWriter:write(message)
    -- assert("table" == type(message))
    local msg_str = c.encode(self._request_type, message)
    self._c_writer:write(msg_str)
end  -- write()

--- Wrap close callback.
-- `function(table|nil, error_str, status_code)` ->
-- `function(string|nil, error_str, status_code)`
local function wrap_close_cb(close_cb, response_type)
    if not close_cb then return nil end
    return function(resp_str, error_str, status_code)
        if not resp_str then
            close_cb(nil, error_str, status_code)
            return
        end

        -- assert("string" == type(resp_str))
        local resp = c.decode(response_type, resp_str)
        if resp then
            close_cb(resp, error_str, status_code)
            return
        end
        -- GRPC_STATUS_INTERNAL = 13
        local err = "Failed to decode response " .. response_type
        close_cb(nil, err, 13)
    end
end  -- wrap_close_cb()

--- Async close and get response.
-- @tparam func|nil close_cb close callback function, or nil to ignore response.
-- `function(table|nil, error_str|nil, status_code)`
-- @usage
-- writer:close(
--     function(response, error_str, status_code)
--         assert(not response or "table" == type(response))
--         assert(not error_str or "string" == type(error_str))
--         assert("number" == type(status_code))
--     end)
function ClientAsyncWriter:close(close_cb)
    -- assert(not close_cb or "function" == type(close_cb))
    self._c_writer:close(wrap_close_cb(close_cb, self._response_type))
end  -- close()

-------------------------------------------------------------------------------
--- Private functions.
-- @section private

return ClientAsyncWriter
