npc_maria = Npc:new("Maria")
npc_maria.hp = 50
npc_maria.max_hp = 50

local conversation = 0

function npc_maria:handleMessage(text)
    if conversation == 0 and string.contains_word(text, "hi") then
        npc_maria:say(chat_global, "Hello, " .. player.name .. "!")
        conversation = 1
    elseif conversation == 1 and string.contains_words(text, { "how", "are", "you" }) then
        npc_maria:say(chat_global, "Хорошо, спасибо! Thank you!")
        npc_maria:say(chat_global, "I could eat some berries, though... Do you think you can collect berries for me?")
        conversation = 2
    elseif conversation == 2 and string.contains_words(text, { "collect", "berries" }) then
        npc_maria:say(chat_global, "Отлично! I'll be waiting until you bring me 5 berries.")
        conversation = 3
    elseif conversation == 3 then
        if player:item_count("berry") < 5 then
            npc_maria:say(chat_global, "Oioioi I want berries so muuch!")
        else
            npc_maria:say(chat_global, "Oooh! Спасибо болшои!")
            player:item_remove("berry", 5)
        end
    end
end

