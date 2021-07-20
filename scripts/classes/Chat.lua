Chat = { name = "", log = {}, log_pos = 1 }
Chat.__index = Chat

function Chat:new(name)
    local chat = { name }
    setmetatable(chat, self)
    self.__index = self

    self.name = name
    self.log = {}
    self.log_pos = 1

    return chat
end

function Chat:log_message(text)
    self.log[self.log_pos] = text
    self.log_pos = self.log_pos + 1
end

