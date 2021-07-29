Npc = {}
Npc.__index = Npc

global_npc_registry = {}

function Npc:new(name)
    local self = setmetatable({}, Npc)

    self.name = name
    self.x = 0
    self.y = 0

    global_npc_registry[name] = self

    return self
end

function Npc:say(chat, text)
    chat:log_message(os.date("[%I:%M:%S] " .. self.name .. ": " .. text))
end

setmetatable(Npc, {__index = Character})

