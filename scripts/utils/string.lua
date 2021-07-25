-- Extend std library's string
string.split = function(str, delim)
    local result = {}

    for match in (str .. delim):gmatch("(.-)" .. delim) do
        table.insert(result, match)
    end
    
    return result
end

string.contains_word = function(str, word)
    local chunks = string.split(str, " ")

    for k, w in ipairs(chunks) do
        if (w == word) then
            return true
        end
    end

    return false
end

