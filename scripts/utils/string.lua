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

string.contains_words = function(str, words)
    local chunks = string.split(str, " ")
    local result = {}

    for k, w in ipairs(words) do
        result[words] = false
    end

    for k, w in ipairs(chunks) do
        if result[w] ~= nil then
            result[w] = true
        end
    end

    for k, w in ipairs(result) do
        if w == false then
            return false
        end
    end

    return true
end

