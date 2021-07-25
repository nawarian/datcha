npc_maria = Npc:new("Maria", 0, 0)

function npc_maria:handleMessage(text)
    local self = npc_maria
    chunks = string.split(text, " ")
    for i, v in ipairs(chunks) do
        if v == "hi" then -- TODO: replace with greetings list
            self:initConversation()
        end
    end
end

function npc_maria:initConversation()
    npc_maria:say(chat_global, "Hello dear! How can I help you?")
end

