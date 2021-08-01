#include "raylib.h"
#include <stdbool.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>

#include "global_variables.h"
#include "macros.h"
#include "w_console.h"

ConsoleState console = { 0 };

void _push_message_to_lua(void);
void _update_console_log_from_lua(void);

void w_console_update(void)
{
    const char *last_unicode_char_pressed;
    int
        char_length
        , bytes_processed;
    unsigned int
        last_char_pressed
        , total_char_count
        , i;

    // Chat log scroll
    console.scroll_pos -= (int) GetMouseWheelMove();
    if (console.scroll_pos > 0) {
        console.scroll_pos = 0;
    } else if (console.scroll_pos <= -console.log_pos) {
        console.scroll_pos = -console.log_pos + 1;
    }

    // Toggle input mode
    if (IsKeyPressed(KEY_ENTER)) {
        console.typing = !console.typing;

        if (!console.typing && console.cursor_pos > 0) {
            _push_message_to_lua();
        }
    }

    // Fetch last console messages from LUA
    _update_console_log_from_lua();

    if (!console.typing) {
        return;
    }

    // Handle typing
    if (console.cursor_pos > 0 && IsKeyPressed(KEY_BACKSPACE)) {
        total_char_count = GetCodepointsCount(console.buff);

        for (i = 0; i < total_char_count;) {
            GetNextCodepoint(&console.buff[i], &bytes_processed);

            i += bytes_processed;
        }

        console.cursor_pos -= bytes_processed;
        while (bytes_processed > 0) {
            console.buff[console.cursor_pos - 1 + bytes_processed--] = 0;
        }
    }

    last_char_pressed = GetCharPressed();
    if (last_char_pressed) {
        last_unicode_char_pressed = CodepointToUtf8(last_char_pressed, &char_length);

        TextCopy(console.buff + console.cursor_pos, last_unicode_char_pressed);
        console.cursor_pos += char_length;
    }
}

void w_console_draw(void)
{
    int
        sw = GetScreenWidth()
        , sh = GetScreenHeight()
        , h = sh * .2 // console's height = 20% of Window's height
        , input_box_y = sh - (h * .2)
        , font_size = 20;

    Rectangle log_area = {
        1,
        sh - h + 1,
        sw - 6,
        h - 2 - (sh - input_box_y)
    };
    
    Color
        c_border = ColorAlpha(WHITE, 0.8)
        , c_bg = ColorAlpha(BLACK, 0.5);

    // Draw main console rec
    DrawRectangleLines(0, sh - h, sw, h, c_border);
    DrawRectangle(1, sh - h + 1, sw - 2, h - 2, c_bg);

    // Draw message log
    BeginScissorMode(log_area.x, log_area.y, log_area.width, log_area.height);
    for (int i = console.log_pos, j = 0; i >= 0; --i, ++j) {
        DrawTextUTF8(
            console.log[i],
            1, 
            input_box_y - ((j + console.scroll_pos) * font_size),
            font_size,
            WHITE
        );
    }
    EndScissorMode();

    // Draw input box
    h = sh * .2; // Consider default box size for Input box
    DrawRectangleLines(1, input_box_y, sw - 2, (h * .2) - 1, c_border);

    if (console.typing) {
        DrawRectangle(1, input_box_y, sw - 1, (h * .2) - 1, ColorAlpha(BLACK, 0.7f));
    }
    
    // Draw cursor
    Vector2 cursor_size = MeasureTextUTF8(console.buff, font_size);
    int cursor_x = cursor_size.x + 4;
    DrawRectangle(
        cursor_x,
        sh - (h * .2),
        2,
        (h * .2) - 1,
        WHITE
    );

    DrawTextUTF8(
        console.buff,
        4,
        sh - (h * .2) + cursor_size.y / 4 + font_size / 4,
        font_size,
        WHITE
    );
}

void _push_message_to_lua(void)
{
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

void _update_console_log_from_lua(void)
{
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
}

