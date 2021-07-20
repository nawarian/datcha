#ifndef global_variables_h
#define global_variables_h

#include "tmx.h"
#include <lua.h>
#include "g_state.h"

extern GameState st;
extern lua_State *lua;

// used by r_tiled.h
extern tmx_map *map;

#endif

