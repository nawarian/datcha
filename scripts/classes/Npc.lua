Npc = { name = "" }
Npc.__index = Npc

global_npc_registry = {}

function Npc:new(name, x, y)
    npc = {}
    setmetatable(npc, self)
    npc.__index = self

    self.name = name
    self.x = x
    self.y = y

    global_npc_registry[name] = npc

    return npc
end

function Npc:say(chat, text)
    chat:log_message(os.date("[%I:%M:%S] " .. self.name .. ": " .. text))
end

