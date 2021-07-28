#include "raylib.h"
#include <lualib.h>

#include "global_variables.h"
#include "macros.h"
#include "g_npc.h"

#define LUA_NPCS_VAR "global_npc_registry"
// Npc npc_registry[100]

void g_npc_init(void)
{
    const char *name;
    unsigned int i = 0
        , hp
        , max_hp;
    Vector2 coords, velocity;

    lua_getglobal(lua, LUA_NPCS_VAR);
    if (!lua_istable(lua, -1)) {
        TraceLog(
            LOG_ERROR,
            "Could not fetch npc registry from lua. Variable name '%s'",
            LUA_NPCS_VAR
        );
    }

    lua_pushnil(lua);
    while (lua_next(lua, -2)) {
        velocity = (Vector2) { 0 };

        // load x,y coords
        lua_getfield(lua, -1, "x");
        lua_getfield(lua, -2, "y");
        coords = (Vector2) {
            lua_tointeger(lua, -2),
            lua_tointeger(lua, -1)
        };
        lua_pop(lua, 2); // unload x,y coords

        // load name, hp, max_hp
        lua_getfield(lua, -1, "name");
        lua_getfield(lua, -2, "hp");
        lua_getfield(lua, -3, "max_hp");
        name = lua_tostring(lua, -3);
        hp = lua_tointeger(lua, -2);
        max_hp = lua_tointeger(lua, -1);
        lua_pop(lua, 3); // unload name, hp, max_hp

        npc_registry[i++] = (Npc) {
            name,
            coords,
            velocity,
            hp,
            max_hp
        };

        lua_pop(lua, 1);
    }
    lua_pop(lua, 1);
}

void g_npc_update(void)
{
}

void g_npc_draw(void)
{
    unsigned int
        i = 0
        , max_width = 16;
    float health_pct;
    Color hp_bar_color;

    for (i = 0; i < 100; ++i) {
        if(npc_registry[i].name == NULL ) {
            break;
        }

        health_pct = (float) npc_registry[i].hp / (float) npc_registry[i].max_hp;

        if (health_pct < .3) {
            hp_bar_color = RED;
        } else if (health_pct < .6) {
            hp_bar_color = ORANGE;
        } else if (health_pct < .8) {
            hp_bar_color = YELLOW;
        } else {
            hp_bar_color = GREEN;
        }

        DrawRectangle(
            npc_registry[i].coords.x,
            npc_registry[i].coords.y - 5,
            max_width * npc_registry[i].hp / npc_registry[i].max_hp,
            3,
            hp_bar_color
        );
        DrawRectangleLinesEx(
            (Rectangle) {
                npc_registry[i].coords.x,
                npc_registry[i].coords.y - 5,
                max_width,
                3
            },
            1,
            BLACK
        );
        DrawTextUTF8(
            npc_registry[i].name,
            npc_registry[i].coords.x - (MeasureTextUTF8(npc_registry[i].name, 8).x / 4),
            npc_registry[i].coords.y - 16,
            8,
            WHITE
        );

        DrawRectangle(
            npc_registry[i].coords.x,
            npc_registry[i].coords.y,
            16,
            16,
            BLUE
        );
    }
}

