npc_maria = Npc:new("Maria", 0, 0)

local conversation = 0

function npc_maria:handleMessage(text)
    if conversation == 0 and string.contains_word(text, "привет") then
        npc_maria:say(chat_global, "Здравствуйте!")
        conversation = 1
    elseif conversation == 1 and text == "как дела" then
        npc_maria:say(chat_global, "Хорошо, спасибо! :)")
    end
end

