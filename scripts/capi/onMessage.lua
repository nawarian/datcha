-- Script: capi/onMessage.lua
-- void onMessage(const char *text)
function onMessage(text)
    console:handleRawMessage(text)
end

