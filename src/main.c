#include "raylib.h"
#include "tmx.h"
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include <stdbool.h>

#include "global_variables.h"
#include "g_state.h"
#include "r_tiled.h"
#include "w_console.h"

Camera2D camera;
GameState st;
lua_State *lua;

bool _load_lua_script(const char *filename);
void _camera_init(void);
void _camera_update();

int main(void)
{
    InitWindow(800, 600, "Datcha - PoC");
    SetTargetFPS(60);

    // initialise LUA
    lua = luaL_newstate();
    luaL_openlibs(lua);
    _load_lua_script("scripts/main.lua");

    // reset global state
    g_state_reset();
    st.running = true;

    // Load TMX test map
    if (!r_tiled_load_map("assets/maps/rpg-sample/island.tmx")) {
        return 1;
    }

    // Camera
    _camera_init();

    while(st.running) {
        st.running = !WindowShouldClose();
        _camera_update();

        // Update widgets
        w_console_update();

        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(DARKBLUE);

                r_tiled_draw();
            EndMode2D();

            w_console_draw();
            DrawFPS(0, 0);
        EndDrawing();
    }

    r_tiled_unload_map();
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
            TraceLog(LOG_INFO, "Loaded file '%s'", filename);
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

Vector2 _get_player_pos()
{
    Vector2 pos = { 0 };

    lua_getglobal(lua, "player");
    if (lua_istable(lua, -1)) {
        lua_getfield(lua, -1, "x");
        lua_getfield(lua, -2, "y");

        // y
        pos.y = lua_tointeger(lua, -1);
        lua_remove(lua, -1);

        // x
        pos.x = lua_tointeger(lua, -1);
        lua_remove(lua, -1);
    } else {
        TraceLog(LOG_ERROR, "Could not fetch LUA's global variable 'player'");
    }

    // clean up stack
    lua_remove(lua, -1);

    return pos;
}

void _camera_init(void)
{
    camera = (Camera2D) { 0 };
    camera.target = (Vector2) { 100, 100 };
    camera.offset = (Vector2) { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

void _camera_update()
{
    camera.offset = (Vector2) { 0, 0 };
    camera.target = _get_player_pos();

    if (IsKeyDown(KEY_A)) {
        camera.target.x -= 5;
    } else if (IsKeyDown(KEY_D)) {
        camera.target.x += 5;
    }

    if (IsKeyDown(KEY_W)) {
        camera.target.y -= 5;
    } else if (IsKeyDown(KEY_S)) {
        camera.target.y += 5;
    }

    if (IsKeyPressed(KEY_UP)) {
        camera.zoom += 0.3f;
    } else if (IsKeyPressed(KEY_DOWN)) {
        camera.zoom -= 0.3f;
    }
}

