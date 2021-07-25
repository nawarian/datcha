#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>

#include "global_variables.h"
#include "w_console.h"

ConsoleState console = { 0 };

void w_console_update(void)
{
    char last_char_pressed;

    console.scroll_pos -= (int) GetMouseWheelMove();
    if (console.scroll_pos > 0) {
        console.scroll_pos = 0;
    } else if (console.scroll_pos <= -console.log_pos) {
        console.scroll_pos = -console.log_pos + 1;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        // User sent a text: handle message and clear buffer
        if (console.typing) {
            // delegate to LUA handler(s)
            lua_getglobal(lua, "onMessage");
            lua_pushstring(lua, console.buff);
            if (lua_pcall(lua, 1, 1, 0) != 0) {
                TraceLog(LOG_ERROR, "Lua error: %s", lua_tostring(lua, -1));
            }

            // clear buffer, cursor and reset scroll
            while (console.cursor_pos > 0) {
                console.buff[--console.cursor_pos] = 0;
                console.scroll_pos = 0;
            }
        }

        console.typing = !console.typing;
    }

    // Fetch last 50 messages from LUA
    lua_getglobal(lua, "chat_global");
    console.log_pos = 0;
    if (lua_istable(lua, -1)) {
        lua_getfield(lua, -1, "log");
        if (lua_istable(lua, -1)) {
            lua_pushnil(lua);

            while (lua_next(lua, -2)) {
                strcpy(console.log[console.log_pos++], lua_tostring(lua, -1));

                lua_pop(lua, 1);
            }
            lua_pop(lua, -1);
        }

        lua_pop(lua, -1);
    }
    lua_pop(lua, -1);

    if (!console.typing) {
        return;
    }

    last_char_pressed = GetCharPressed();
    if (last_char_pressed) {
        console.buff[console.cursor_pos++] = last_char_pressed;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        console.buff[console.cursor_pos--] = 0;
    }

    if (console.cursor_pos < 0) {
        console.cursor_pos = 0;
    }

    if (console.cursor_pos > 63) {
        console.cursor_pos = 63;
    }
}

void w_console_draw(void)
{
    int sw = GetScreenWidth(),
        sh = GetScreenHeight(),
        w = sw * .8, // console's width = 80% of Window's width
        h = sh * .2, // console's height = 20% of Window's height
        input_box_y = sh - (h * .2),
        font_size = 20,
        margin_x = 5;
    Rectangle log_area = {
        1 + margin_x,
        sh - h + 1,
        w - 6 - margin_x,
        h - 2 - (sh - input_box_y)
    };
    
    // If typing, increase the console's height
    if (console.typing) {
        h = sh * .3;
        log_area.y = sh - h + 1;
        log_area.height = h - 2 - (sh - input_box_y);
    }

    Color c_border = ColorAlpha(WHITE, 0.8),
          c_bg = ColorAlpha(BLACK, 0.5);

    // Draw main console rec
    DrawRectangleLines(0, sh - h, w, h, c_border); 
    DrawRectangle(1, sh - h + 1, w - 2, h - 2, c_bg); 

    // Draw message log
    BeginScissorMode(log_area.x, log_area.y, log_area.width, log_area.height);
    for (int i = console.log_pos, j = 0; i >= 0; --i, ++j) {
        DrawText(
            console.log[i],
            1 + margin_x,
            input_box_y - ((j + console.scroll_pos) * font_size),
            font_size,
            WHITE
        );
    }
    EndScissorMode();

    // Draw input box
    h = sh * .2; // Consider default box size for Input box
    DrawRectangleLines(1, input_box_y, w - 2, (h * .2) - 1, c_border);
    
    // Draw cursor
    int cursor_x = MeasureText(console.buff, font_size) + 4 + margin_x;
    DrawRectangle(
        cursor_x,
        sh - (h * .2),
        2,
        (h * .2) - 1,
        WHITE
    );

    DrawText(
        console.buff,
        4 + margin_x,
        sh - (h * .2),
        font_size,
        WHITE
    );
}

