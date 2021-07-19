#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "w_console.h"

struct {
    bool typing;
    int cursor_pos;
    int log_pos;
    int scroll_pos;
    char buff[64];
    char log[50][64]; // last 50 messages
} ConsoleState = { 0 };

void w_console_update(void)
{
    char last_char_pressed;

    ConsoleState.scroll_pos -= (int) GetMouseWheelMove();
    if (ConsoleState.scroll_pos > 0) {
        ConsoleState.scroll_pos = 0;
    } else if (ConsoleState.scroll_pos <= -ConsoleState.log_pos) {
        ConsoleState.scroll_pos = -ConsoleState.log_pos + 1;
    }

    if (IsKeyPressed(KEY_ENTER)) {
        // User sent a text: handle message and clear buffer
        if (ConsoleState.typing) {
            // Broadcast (debug only)
            TraceLog(LOG_INFO, "[Broadcast] %s", ConsoleState.buff);

            // log message to game console
            strcpy(ConsoleState.log[ConsoleState.log_pos++], ConsoleState.buff);

            // delegate to LUA handler(s)
            // TODO

            // clear buffer, cursor and reset scroll
            while (ConsoleState.cursor_pos > 0) {
                ConsoleState.buff[--ConsoleState.cursor_pos] = 0;
                ConsoleState.scroll_pos = 0;
            }
        }

        ConsoleState.typing = !ConsoleState.typing;
    }

    if (!ConsoleState.typing) {
        return;
    }

    last_char_pressed = GetCharPressed();
    if (last_char_pressed) {
        ConsoleState.buff[ConsoleState.cursor_pos++] = last_char_pressed;
    }

    if (IsKeyPressed(KEY_BACKSPACE)) {
        ConsoleState.buff[ConsoleState.cursor_pos--] = 0;
    }

    if (ConsoleState.cursor_pos < 0) {
        ConsoleState.cursor_pos = 0;
    }

    if (ConsoleState.cursor_pos > 63) {
        ConsoleState.cursor_pos = 63;
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
    if (ConsoleState.typing) {
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
    for (int i = ConsoleState.log_pos, j = 0; i >= 0; --i, ++j) {
        DrawText(
            ConsoleState.log[i],
            1 + margin_x,
            input_box_y - ((j + ConsoleState.scroll_pos) * font_size),
            font_size,
            WHITE
        );
    }
    EndScissorMode();

    // Draw input box
    h = sh * .2; // Consider default box size for Input box
    DrawRectangleLines(1, input_box_y, w - 2, (h * .2) - 1, c_border);
    
    // Draw cursor
    int cursor_x = MeasureText(ConsoleState.buff, font_size) + 4 + margin_x;
    DrawRectangle(
        cursor_x,
        sh - (h * .2),
        2,
        (h * .2) - 1,
        WHITE
    );

    DrawText(
        ConsoleState.buff,
        4 + margin_x,
        sh - (h * .2),
        font_size,
        WHITE
    );
}

