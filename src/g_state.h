#ifndef g_state_h
#define g_state_h

#include <stdbool.h>

typedef struct GameState {
    bool running;
} GameState;

void g_state_reset(void);

#endif

