#include "raylib.h"

#include "global_variables.h"
#include "g_state.h"

GameState st;

int main(void)
{
    InitWindow(800, 600, "Datcha - PoC");
    SetTargetFPS(60);

    g_state_reset();
    st.running = true;
    while(st.running) {
        st.running = !WindowShouldClose();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawFPS(0, 0);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

