Character = { name = "" }
Character.__index = Character

function Character:new(name)
    character = character or { }
    setmetatable(character, self)
    character.__index = self

    self.name = name

    return character
end

function Character:say(chat, text)
    chat:log_message(os.date("[%I:%M:%S] " .. self.name .. ": " .. text))
end

