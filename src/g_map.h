#ifndef g_map_h
#define g_map_h

#include <stdbool.h>

bool g_map_load(const char *filename);
void g_map_unload(void);
void g_map_draw(void);
tmx_object* g_map_object_get_by_type(const char *type);

#endif

