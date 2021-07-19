#include "raylib.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include <stdbool.h>

#include "global_variables.h"
#include "g_state.h"
#include "w_console.h"

GameState st;
lua_State *lua;

bool _load_lua_script(const char *filename);

int main(void)
{
    InitWindow(800, 600, "Datcha - PoC");
    SetTargetFPS(60);

    // initialise LUA
    lua = luaL_newstate();
    luaL_openlibs(lua);
    _load_lua_script("scripts/console.lua");

    // reset global state
    g_state_reset();
    st.running = true;
    while(st.running) {
        st.running = !WindowShouldClose();
        w_console_update();

        BeginDrawing();
            ClearBackground(DARKBLUE);
            DrawFPS(0, 0);

            w_console_draw();
        EndDrawing();
    }

    lua_close(lua);
    CloseWindow();

    return 0;
}

bool _load_lua_script(const char *filename)
{
    int result = luaL_loadfile(lua, filename);
    char *reason_phrase = NULL;

    switch (result) {
        case LUA_OK:
            TraceLog(LOG_INFO, "Loaded file 'console.lua'");
            lua_pcall(lua, 0, 0, 0);
            return true;
        default:
            if (result == LUA_ERRSYNTAX) reason_phrase = "Syntax error";
            if (result == LUA_ERRMEM) reason_phrase = "Out of memory";
            if (result == LUA_ERRFILE) reason_phrase = "File error";

            TraceLog(
                LOG_ERROR,
                "Could not load LUA script '%s'. Reason: %s.",
                filename,
                reason_phrase
            );

            return false;
    }
}

