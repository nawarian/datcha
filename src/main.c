#include "raylib.h"

#include "global_variables.h"
#include "g_state.h"
#include "w_console.h"

GameState st;

int main(void)
{
    InitWindow(800, 600, "Datcha - PoC");
    SetTargetFPS(60);

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

    CloseWindow();

    return 0;
}

