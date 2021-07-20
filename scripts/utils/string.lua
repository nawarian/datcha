-- Extend std library's string
string.split = function(str, delim)
    result = {}

    for match in (str .. delim):gmatch("(.-)" .. delim) do
        table.insert(result, match)
    end
    
    return result
end

