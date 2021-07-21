#include "raylib.h"

#include "global_variables.h"
#include "g_player.h"

void _sync_player_state_lua(void);
void _update_player_state_lua(void);

void g_player_init(void)
{
    st.player = (Player) {{ 0, 0 }, { 16, 16 }};
    _sync_player_state_lua();
}

void g_player_update(void)
{
    _sync_player_state_lua();

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

    _update_player_state_lua();
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

void _sync_player_state_lua(void)
{
    lua_getglobal(lua, "player");
    if (lua_istable(lua, -1)) {
        lua_getfield(lua, -1, "x");
        lua_getfield(lua, -2, "y");

        // y
        st.player.coords.y = lua_tointeger(lua, -1);
        lua_remove(lua, -1);

        // x
        st.player.coords.x = lua_tointeger(lua, -1);
        lua_remove(lua, -1);
    } else {
        TraceLog(LOG_ERROR, "Cannot sync player state from LUA");
    }

    // Clean up lua stack
    lua_remove(lua, -1);
}

void _update_player_state_lua(void)
{
    lua_getglobal(lua, "player");
    if (lua_istable(lua, -1)) {
        lua_pushnumber(lua, st.player.coords.x);
        lua_setfield(lua, -2, "x");

        lua_pushnumber(lua, st.player.coords.y);
        lua_setfield(lua, -2, "y");
    } else {
        TraceLog(LOG_ERROR, "Cannot sync player state from LUA");
    }

    // Clean up lua stack
    lua_remove(lua, -1);
}

