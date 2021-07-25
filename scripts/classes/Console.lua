Console = { }
Console.__index = Console

function Console:new()
    c = c or {}
    setmetatable(c, self)
    self.__index = self

    return c
end

function Console:handleRawMessage(text)
    player:say(chat_global, text)

    -- Check npcs in voice reach radius
    radius = 16 * 4
    local pos = Vec2:new(player.x, player.y)
    for i, npc in pairs(global_npc_registry) do
        local npc_pos = Vec2:new(npc.x, npc.y)

        if collision.in_circle(npc_pos, pos, radius) then
            npc:handleMessage(text)
        end
    end
end

