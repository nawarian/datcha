#ifndef g_npc_h
#define g_npc_h

#include "raylib.h"

typedef struct Npc {
    const char *name;
    Vector2 coords;
    Vector2 velocity;
    unsigned int
        hp
        , max_hp;
} Npc;

void g_npc_init(void);
void g_npc_update(void);
void g_npc_draw(void);

#endif

