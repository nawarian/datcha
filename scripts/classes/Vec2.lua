Vec2 = { x = 0, y = 0 }
Vec2.__index = Vec2

function Vec2:new(x, y)
    vec2 = {}
    setmetatable(vec2, self)

    vec2.x = x
    vec2.y = y

    return vec2
end
