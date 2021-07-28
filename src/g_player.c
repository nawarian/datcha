#include "raylib.h"
#include <string.h>

#include "global_variables.h"
#include "macros.h"
#include "g_player.h"
#include "g_map.h"
#include "u_lua.h"

void g_player_init(void)
{
    unsigned int player_found = 0;
    tmx_object* player_obj;
    st.player = (Player) {"", { 0, 0 }, { 4, 4 }, 0, 0};
    st.player.name = u_lua_field_get_string("player", "name");
    st.player.coords.x = u_lua_field_get_int("player", "x");
    st.player.coords.y = u_lua_field_get_int("player", "y");

    player_obj = g_map_object_get_by_type("start", &player_found);
    if (player_found == 0) {
        TraceLog(LOG_ERROR, "No player object placed on map");
    }

    u_lua_field_set_int("player", "x", (int) player_obj->x - 8);
    u_lua_field_set_int("player", "y", (int) player_obj->y - 8);
}

void g_player_update(void)
{
    Vector2 world_lim = {
        (map->width - 1) * map->tile_width,
        (map->height - 1) * map->tile_height
    };

    st.player.coords.x = u_lua_field_get_int("player", "x");
    st.player.coords.y = u_lua_field_get_int("player", "y");
    st.player.hp = u_lua_field_get_int("player", "hp");
    st.player.max_hp = u_lua_field_get_int("player", "max_hp");

    if (!console.typing) {
        if (IsKeyDown(KEY_A)) {
            st.player.coords.x -= st.player.velocity.x;
        } else if (IsKeyDown(KEY_D)) {
            st.player.coords.x += st.player.velocity.x;
        }

        if (IsKeyDown(KEY_W)) {
            st.player.coords.y -= st.player.velocity.y;
        } else if (IsKeyDown(KEY_S)) {
            st.player.coords.y += st.player.velocity.y;
        }
    }

    if (st.player.coords.x > world_lim.x) {
        st.player.coords.x = world_lim.x;
    } else if (st.player.coords.x < 0) {
        st.player.coords.x = 0;
    }

    if (st.player.coords.y > world_lim.y) {
        st.player.coords.y = world_lim.y;
    } else if (st.player.coords.y < 0) {
        st.player.coords.y = 0;
    }

    u_lua_field_set_int("player", "x", st.player.coords.x);
    u_lua_field_set_int("player", "y", st.player.coords.y);
}

void g_player_draw(void)
{
    const unsigned int max_width = 16;
    const float health_pct = (float) st.player.hp / (float) st.player.max_hp;
    Color hp_bar_color;

    DrawRectangle(
        st.player.coords.x,
        st.player.coords.y,
        16,
        16,
        RED 
    );

    if (health_pct < .3) {
        hp_bar_color = RED;
    } else if (health_pct < .6) {
        hp_bar_color = ORANGE;
    } else if (health_pct < .8) {
        hp_bar_color = YELLOW;
    } else {
        hp_bar_color = GREEN;
    }

    DrawTextUTF8(
        st.player.name,
        st.player.coords.x - (MeasureTextUTF8(st.player.name, 8).x / 4),
        st.player.coords.y - 16,
        8,
        WHITE
    );
    DrawRectangle(
        st.player.coords.x,
        st.player.coords.y - 5,
        max_width * st.player.hp / st.player.max_hp,
        3,
        hp_bar_color
    );
    DrawRectangleLinesEx(
        (Rectangle) {
            st.player.coords.x,
            st.player.coords.y - 5,
            max_width,
            3
        },
        1,
        BLACK
    );
}

