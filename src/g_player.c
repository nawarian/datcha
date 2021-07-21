#include "raylib.h"
#include <string.h>

#include "global_variables.h"
#include "g_player.h"
#include "g_map.h"
#include "u_lua.h"

void g_player_init(void)
{
    tmx_object* player_obj;
    st.player = (Player) {{ 0, 0 }, { 16, 16 }};
    st.player.coords.x = u_lua_field_get_int("player", "x");
    st.player.coords.y = u_lua_field_get_int("player", "y");

    player_obj = g_map_object_get_by_type("start");
    if (player_obj == NULL) {
        TraceLog(LOG_ERROR, "No player object placed on map");
    }

    st.player.coords.x = (int) player_obj->x - 8;
    st.player.coords.y = (int) player_obj->y - 8;

    u_lua_field_set_int("player", "x", st.player.coords.x);
    u_lua_field_set_int("player", "y", st.player.coords.y);
}

void g_player_update(void)
{
    st.player.coords.x = u_lua_field_get_int("player", "x");
    st.player.coords.y = u_lua_field_get_int("player", "y");

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

    u_lua_field_set_int("player", "x", st.player.coords.x);
    u_lua_field_set_int("player", "y", st.player.coords.y);
}

void g_player_draw(void)
{
    DrawRectangle(
        st.player.coords.x,
        st.player.coords.y,
        16,
        16,
        BLUE
    );
}

