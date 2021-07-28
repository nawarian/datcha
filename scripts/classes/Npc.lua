Npc = {}
Npc.__index = Npc

global_npc_registry = {}

function Npc:new(name, x, y)
    local self = setmetatable({}, Npc)

    self.name = name
    self.x = x
    self.y = y

    global_npc_registry[name] = self

    return self
end

function Npc:say(chat, text)
    chat:log_message(os.date("[%I:%M:%S] " .. self.name .. ": " .. text))
end

setmetatable(Npc, {__index = Character})

