Character = {
    name = ""
    , x = 0
    , y = 0
    , hp = 50
    , max_hp = 50
    , items = {}
}
Character.__index = Character

function Character:new(name, x, y, hp, max_hp)
    local self = setmetatable({}, Character);

    self.name = name
    self.x = x
    self.y = y
    self.items = {}

    return self
end

function Character:say(chat, text)
    chat:log_message(os.date("[%I:%M:%S] " .. self.name .. ": " .. text))
end

function Character:take_damage(damage)
    self.hp = self.hp - damage

    self.hp = math.max(self.hp, 0)
    self.hp = math.min(self.hp, self.max_hp)
end

function Character:item_count(item)
    if player.items[item] ~= nil then
        return player.items[item]
    end

    return 0
end

function Character:item_add(item, count)
    if self:item_count(item) == 0 then
        player.items[item] = 0
    end

    player.items[item] = player.items[item] + count
end

function Character:item_remove(item, count)
    if self:item_count(item) >= count then
        print("Removing " .. count .. " from " .. self:item_count(item))
        player.items[item] = player.items[item] - count
    end
end

