#ifndef g_player_h
#define g_player_h

#include "raylib.h"

typedef struct Player {
    const char *name;
    Vector2 coords;
    Vector2 velocity;
    unsigned int
        hp
        , max_hp;
} Player;

void g_player_init(void);
void g_player_update(void);
void g_player_draw(void);

#endif

