#include "raylib.h"
#include <stdbool.h>
#include <stdio.h>

#include "w_console.h"

struct {
    bool typing;
    int cursor_pos;
    char buff[64];
} ConsoleState = { 0 };

void w_console_update(void)
{
    char last_char_pressed;

    if (IsKeyPressed(KEY_ENTER)) {
        // User sent a text, broadcast message and clear buffer
        if (ConsoleState.typing) {
            // broadcast
            TraceLog(LOG_INFO, "[Broadcast] %s", ConsoleState.buff);

            // clear buffer & cursor
            while (ConsoleState.cursor_pos > 0) {
                ConsoleState.buff[--ConsoleState.cursor_pos] = 0;
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
        font_size = 20;
    
    // If typing, increase the console's height
    if (ConsoleState.typing) {
        h = sh * .3;
    }

    Color c_border = ColorAlpha(WHITE, 0.8),
          c_bg = ColorAlpha(BLACK, 0.5);

    // Draw main console rec
    DrawRectangleLines(0, sh - h, w, h, c_border); 
    DrawRectangle(1, sh - h + 1, w - 2, h - 2, c_bg); 

    // Draw input box
    h = sh * .2; // Consider default box size for Input box
    DrawRectangleLines(1, sh - (h * .2), w - 2, (h * .2) - 1, c_border);
    
    // Draw cursor
    int cursor_x = MeasureText(ConsoleState.buff, font_size) + 4;
    DrawRectangle(
        cursor_x,
        sh - (h * .2),
        2,
        (h * .2) - 1,
        WHITE
    );

    DrawText(
        ConsoleState.buff,
        4,
        sh - (h * .2),
        font_size,
        WHITE
    );
}

