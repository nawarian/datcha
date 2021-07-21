#ifndef g_state_h
#define g_state_h

#include <stdbool.h>
#include "g_player.h"

typedef struct GameState {
    bool running;
    Player player;
} GameState;

void g_state_reset(void);

#endif

