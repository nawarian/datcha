#include "raylib.h"
#include "tmx.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "global_variables.h"
#include "g_map.h"
#include "u_lua.h"

#define LINE_THICKNESS 2.5

tmx_map *map = NULL;

// Private functions
void* _tmx_texture_load(const char *filename);
void _tmx_texture_free(void *ptr);
Color _int_to_color(int color);
void _draw_image_layer(tmx_image *image);
void _draw_tile(
    void *image,
    unsigned int sx,
    unsigned int sy,
    unsigned int sw,
    unsigned int sh,
    unsigned int dx,
    unsigned int dy,
    float opacity
);
void _draw_layer(tmx_map *map, tmx_layer *layer);
void _draw_polyline(
    double offset_x,
    double offset_y,
    double **points,
    int points_count,
    Color color
);    
void _draw_polygon(
    double offset_x,
    double offset_y,
    double **points,
    int points_count,
    Color color
);
void _draw_objects(tmx_object_group *objgr);
void _draw_all_layers(tmx_map *map, tmx_layer *layers);
void _draw_all_layers(tmx_map *map, tmx_layer *layers);

// Implement header functions
bool g_map_load(const char *filename)
{
    tmx_object *npcs;
    tmx_property *script_prop;
    unsigned int npc_count = 0;
    char *script_path,
         *global_variable_name;
    tmx_img_load_func = _tmx_texture_load;
    tmx_img_free_func = _tmx_texture_free;

    map = tmx_load(filename);

    if (map == NULL) {
        tmx_perror("Could not load map");
        return false;
    }

    // Load map NPCs
    npcs = g_map_object_get_by_type("npc", &npc_count);
    while (npc_count > 0) {
        script_prop = tmx_get_property(npcs->properties, "script");
        if (script_prop == NULL) {
            TraceLog(
                LOG_ERROR,
                "Found NPC '%s' at (%d, %d) without a 'script' property. Every NPC must have a script.",
                npcs->name,
                (int) npcs->x,
                (int) npcs->y
            );
        }
        script_path = script_prop->value.string;

        script_prop = tmx_get_property(npcs->properties, "global_name");
        if (script_prop == NULL) {
            TraceLog(
                LOG_ERROR,
                "Found NPC '%s' at (%d, %d) without a 'global_name' property. Every NPC must have a global_name.",
                npcs->name,
                (int) npcs->x,
                (int) npcs->y
            );
        }
        global_variable_name = script_prop->value.string;

        TraceLog(
            LOG_INFO,
            "Found NPC '%s' at (%d, %d) with script '%s' initialised in global variable '%s'",
            npcs->name,
            (int) npcs->x,
            (int) npcs->y,
            script_path,
            global_variable_name
        );

        // Load NPC file
        u_lua_dofile(script_path);

        // Adjust NPC's coordinates
        u_lua_field_set_int(global_variable_name, "x", (int) npcs->x);
        u_lua_field_set_int(global_variable_name, "y", (int) npcs->y);

        ++npcs;
        --npc_count;
    }

    return true;
}

void g_map_unload(void)
{
    tmx_map_free(map);
}

void g_map_draw(void)
{
    ClearBackground(_int_to_color(map->backgroundcolor));
    _draw_all_layers(map, map->ly_head);
}

tmx_object* g_map_object_get_by_type(const char *type, unsigned int *ret_count)
{
    tmx_layer *layer = map->ly_head;
    tmx_object *object, *result;
    unsigned int i = 0;

    while (layer) {
        if (layer->type != L_OBJGR) {
            layer = layer->next;
            continue;
        }

        object = layer->content.objgr->head;
        while (object) {
            if (object->type != NULL && strcmp(object->type, type) == 0) {
                ++*ret_count;
            }
            object = object->next;
        }

        layer = layer->next;
    }

    layer = map->ly_head;
    result = malloc(sizeof(tmx_object) * *ret_count);
    while (layer) {
        if (layer->type != L_OBJGR) {
            layer = layer->next;
            continue;
        }

        object = layer->content.objgr->head;
        while (object) {
            if (object->type != NULL && strcmp(object->type, type) == 0) {
                result[i++] = *object;
            }
            object = object->next;
        }

        layer = layer->next;
    }

    return result;
}

// Implement private functions
void* _tmx_texture_load(const char *filename)
{
    Texture2D *tex = malloc(sizeof(Texture2D));
    *tex = LoadTexture(filename);

    return tex;
}

void _tmx_texture_free(void *ptr)
{
    UnloadTexture(*((Texture2D*) ptr));
    free(ptr);
}

Color _int_to_color(int color)
{
    tmx_col_bytes res = tmx_col_to_bytes(color);

    return *((Color*) &res);
}

void _draw_image_layer(tmx_image *image)
{
    Texture2D *texture = (Texture2D*) image->resource_image;
    DrawTexture(*texture, 0, 0, WHITE);
}

void _draw_tile(
    void *image,
    unsigned int sx,
    unsigned int sy,
    unsigned int sw,
    unsigned int sh,
    unsigned int dx,
    unsigned int dy,
    float opacity
) {
    DrawTextureRec(
        *(Texture2D*) image,
        (Rectangle) { sx, sy, sw, sh },
        (Vector2) { dx, dy },
        ColorAlpha(WHITE, opacity)
    );
}

void _draw_layer(tmx_map *map, tmx_layer *layer)
{
    unsigned long i, j;
    unsigned int gid, x, y, w, h, flags;
    float op;
    tmx_tileset *ts;
    tmx_image *im;
    void* image;

    op = layer->opacity;

    for (i = 0; i < map->height; ++i) {
        for (j = 0; j < map->width; ++j) {
            gid = (layer->content.gids[(i * map->width) + j])
                & TMX_FLIP_BITS_REMOVAL;
            if (map->tiles[gid] != NULL) {
                ts = map->tiles[gid]->tileset;
                im= map->tiles[gid]->image;
                x = map->tiles[gid]->ul_x;
                y = map->tiles[gid]->ul_y;
                w = ts->tile_width;
                h = ts->tile_height;

                if (im) {
                    image = im->resource_image;
                } else {
                    image = ts->image->resource_image;
                }

                flags = (layer->content.gids[(i * map->width) + j]) & ~TMX_FLIP_BITS_REMOVAL;
                _draw_tile(
                    image,
                    x,
                    y,
                    w,
                    h,
                    j * ts->tile_width,
                    i * ts->tile_height,
                    op
                );
            }
        }
    }
}

void _draw_polyline(
    double offset_x,
    double offset_y,
    double **points,
    int points_count,
    Color color
) {
    int i;

    for (i = 1; i < points_count; ++i) {
        DrawLineEx(
            (Vector2) { offset_x + points[i - 1][0], offset_y + points[i - 1][1] },
            (Vector2) { offset_x + points[i][0], offset_y + points[i][1] },
            LINE_THICKNESS,
            color
        );
    }
}

void _draw_polygon(
    double offset_x,
    double offset_y,
    double **points,
    int points_count,
    Color color
) {
    _draw_polyline(offset_x, offset_y, points, points_count, color);
    if (points_count > 2) {
        DrawLineEx(
            (Vector2) { offset_x + points[0][0], offset_y + points[0][1] },
            (Vector2) { offset_x + points[points_count - 1][0], offset_y + points[points_count - 1][1] },
            LINE_THICKNESS,
            color
        );
    }
}

void _draw_objects(tmx_object_group *objgr)
{
    tmx_object *head = objgr->head;
    Color color = _int_to_color(objgr->color);

    while (head) {
        if (head->visible) {
            if (head->obj_type == OT_SQUARE) {
                DrawRectangleLinesEx(
                    (Rectangle) { head->x, head->y, head->width, head->height },
                    (int) LINE_THICKNESS,
                    color
                );
            } else if (head->obj_type == OT_POLYGON) {
                _draw_polygon(head->x, head->y, head->content.shape->points, head->content.shape->points_len, color);
            } else if (head->obj_type == OT_POLYLINE) {
                _draw_polyline(head->x, head->y, head->content.shape->points, head->content.shape->points_len, color);
            } else if (head->obj_type == OT_ELLIPSE) {
                DrawEllipseLines(
                    head->x + head->width / 2.0,
                    head->y + head->height / 2.0,
                    head->width / 2.0,
                    head->height / 2.0,
                    color
                );
            }
        }

        head = head->next;
    }
}

void _draw_all_layers(tmx_map *map, tmx_layer *layers)
{
    while (layers) {
        if (layers->visible) {
            if (layers->type == L_GROUP) {
                _draw_all_layers(map, layers->content.group_head);
            } else if (layers->type == L_OBJGR) {
                _draw_objects(layers->content.objgr);
            } else if (layers->type == L_IMAGE) {
                _draw_image_layer(layers->content.image);
            } else if (layers->type == L_LAYER) {
                _draw_layer(map, layers);
            }
        }

        layers = layers->next;
    }
}

