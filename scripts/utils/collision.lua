collision = {}

collision.in_circle = function(point, center, radius)
    local dx = math.abs(point.x - center.x)
    local dy = math.abs(point.y - center.y)
    return not(dx > radius or dy > radius)
end

