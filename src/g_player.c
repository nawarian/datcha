#include "raylib.h"
#include <string.h>

#include "global_variables.h"
#include "g_player.h"
#include "u_lua.h"

void g_player_init(void)
{
    st.player = (Player) {{ 0, 0 }, { 16, 16 }};
    st.player.coords.x = u_lua_field_get_int("player", "x");
    st.player.coords.y = u_lua_field_get_int("player", "y");

    // fetch player initial position from tmx_map
    tmx_layer *layers = map->ly_head;
    tmx_object *objects;
    while (layers) {
        if (!layers->visible || layers->type != L_OBJGR) {
            layers = layers->next;
            continue;
        }

        // Fetch objects
        objects = layers->content.objgr->head;
        while (objects) {
            if (!objects->visible) {
                objects = objects->next;
                continue;
            }

            if (strcmp(objects->type, "start") == 0) {
                st.player.coords.x = (int) objects->x - 8;
                st.player.coords.y = (int) objects->y - 8;
            }

            objects = objects->next;
        }

        layers = layers->next;
    }

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

