Character = {
    name = ""
    , x = 0
    , y = 0
    , hp = 50
    , max_hp = 50
}
Character.__index = Character

function Character:new(name, x, y, hp, max_hp)
    character = character or { }
    setmetatable(character, self)
    character.__index = self

    self.name = name
    self.x = x
    self.y = y

    return character
end

function Character:say(chat, text)
    chat:log_message(os.date("[%I:%M:%S] " .. self.name .. ": " .. text))
end

function Character:take_damage(damage)
    self.hp = self.hp - damage

    self.hp = math.max(self.hp, 0)
    self.hp = math.min(self.hp, self.max_hp)
end

