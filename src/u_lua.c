#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "global_variables.h"
#include "u_lua.h"

void u_lua_dofile(const char *filename)
{
    int res = luaL_loadfile(lua, filename);
    char *reason = NULL;

    // Loaded fine, early return 
    if (res == LUA_OK) {
        TraceLog(LOG_INFO, "Loaded file '%s'", filename);
        lua_pcall(lua, 0, 0, 0);
        return;
    }

    // Something happened, let's log and die
    switch (res) {
        case LUA_ERRSYNTAX:
            reason = "Syntax error";
            break;
        case LUA_ERRMEM:
            reason = "Out of memory";
            break;
        case LUA_ERRFILE:
            reason = "File error";
            break;
        default: // Should never happen
            reason = "Unknown reason";
            break;
    }

    TraceLog(
        LOG_ERROR,
        "Could not load LUA script '%s'. Reason: %s.",
        filename,
        reason
    );
}

int u_lua_field_get_int(const char *table, const char *field)
{
    int res = 0;

    lua_getglobal(lua, table);
    if (lua_istable(lua, -1)) {
        lua_getfield(lua, -1, field);
        res = lua_tointeger(lua, -1);
        lua_remove(lua, -1);
    } else {
        TraceLog(
            LOG_ERROR,
            "Could not fetch LUA field '%s.%s'",
            table,
            field
        );
    }

    // Clean up stack
    lua_remove(lua, -1);

    return res;
}

void u_lua_field_set_int(const char *table, const char *field, int val)
{
    lua_getglobal(lua, table);
    if (lua_istable(lua, -1)) {
        lua_pushnumber(lua, val);
        lua_setfield(lua, -2, field);
    } else {
        TraceLog(
            LOG_ERROR,
            "Could not fetch LUA field '%s.%s'",
            table,
            field
        );
    }

    // Clean up stack
    lua_remove(lua, -1);
}

