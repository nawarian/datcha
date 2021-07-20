#ifndef r_tiled_h
#define r_tiled_h

#include <stdbool.h>

bool r_tiled_load_map(const char *filename);
void r_tiled_unload_map(void);
void r_tiled_draw(void);

#endif

