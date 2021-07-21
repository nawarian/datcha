#ifndef g_player_h
#define g_player_h

#include "raylib.h"

typedef struct Player {
    Vector2 coords;
    Vector2 velocity;
} Player;

void g_player_init(void);
void g_player_update(void);
void g_player_draw(void);

#endif

