#ifndef u_lua_h
#define u_lua_h

void u_lua_dofile(const char *filename);

/* TODO: make "field" a variadic argument */
int u_lua_field_get_int(const char *table, const char *field);

/* TODO: make "field" a variadic argument */
void u_lua_field_set_int(const char *table, const char *field, int val);

#endif

