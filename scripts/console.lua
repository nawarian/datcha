-- Script: console.lua
-- The C engine hooks into this script by calling "onMessage()"
function onMessage(text)
    print("[Broadcast] " .. text)
end
-- End

