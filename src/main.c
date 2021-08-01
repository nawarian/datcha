#include "raylib.h"
#include <lualib.h>
#include <lauxlib.h>
#include <string.h>
#include <stdbool.h>

#include "global_variables.h"
#include "macros.h"
#include "g_player.h"
#include "g_npc.h"
#include "g_state.h"
#include "g_map.h"
#include "u_lua.h"
#include "w_console.h"

Camera2D camera;
GameState st;
lua_State *lua;
Font font;
Npc npc_registry[100] = { 0 };

void _camera_init(void);
void _camera_update();

int main(void)
{
    InitWindow(1440, 1080, "Datcha - PoC");
    SetTargetFPS(60);

    // load utf-8 compatible font
    font = LoadFont("assets/fonts/dejavu.fnt");

    // initialise LUA
    lua = luaL_newstate();
    luaL_openlibs(lua);
    u_lua_dofile("scripts/main.lua");

    // reset global state
    g_state_reset();
    st.running = true;

    // Load TMX test map
    if (!g_map_load("assets/maps/rpg-sample/island_bigger.tmx")) {
        return 1;
    }

    g_player_init();
    g_npc_init();

    // Camera
    _camera_init();

    while(st.running) {
        st.running = !WindowShouldClose();
        _camera_update();

        // Update game state
        g_player_update();
        g_npc_update();

        // Update widgets
        w_console_update();

        BeginDrawing();
            BeginMode2D(camera);
                ClearBackground(DARKBLUE);

                g_map_draw();
                g_npc_draw();
                g_player_draw();
            EndMode2D();

            w_console_draw();

            // Draw debug info
            DrawFPS(0, 0);
        EndDrawing();
    }

    g_map_unload();
    lua_close(lua);
    CloseWindow();

    return 0;
}

void _camera_init(void)
{
    camera = (Camera2D) { 0 };
    camera.target = st.player.coords;
    camera.offset = (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    camera.rotation = 0.0f;
    camera.zoom = 2.5f;
}

void _camera_update()
{
    camera.offset = (Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 2 };
    camera.target = st.player.coords;

    if (IsKeyPressed(KEY_UP)) {
        camera.zoom += 0.3f;
    } else if (IsKeyPressed(KEY_DOWN)) {
        camera.zoom -= 0.3f;
    }
}

