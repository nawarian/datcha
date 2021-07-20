Console = { }

function Console:new()
    c = c or {}
    setmetatable(c, self)
    self.__index = self

    return c
end

function Console:handleRawMessage(text)
    print("[Broadcast] " .. text)
end

