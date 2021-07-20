Character = { name = "", x = 0, y = 0 }
Character.__index = Character

function Character:new(name, x, y)
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

