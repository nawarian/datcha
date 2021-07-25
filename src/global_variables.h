#ifndef global_variables_h
#define global_variables_h

#include "raylib.h"
#include "tmx.h"
#include <lua.h>
#include "g_state.h"
#include "w_console.h"

extern GameState st;
extern lua_State *lua;
extern Font font;

// used by r_tiled.h
extern tmx_map *map;

// used by w_console.h
extern ConsoleState console;

#endif

