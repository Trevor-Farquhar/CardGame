#ifndef ENTITY_H
#define ENTITY_H

#include <basetypes.h>
#include <raylib.h>
#include "arena.h"

#define ENTITY_NAME_SIZE 64



typedef struct EffectFlags EffectFlags;
struct EffectFlags {
  U8 is_on_fire: 1;
  U8 is_iced: 1;
  U8 is_greesed: 1;
  U8 is_acidified: 1;
  U8 is_wet: 1;
  U8 is_electrified: 1;
  U8 padding: 2;
};


typedef struct EntityFlags EntityFlags;
struct EntityFlags {
  U8 is_playable: 1;
  U8 is_enemyable: 1;
  U8 is_flamable: 1;
  U8 is_freezable: 1;
  U8 is_wetable: 1; // is it able to get wet
  U8 padding : 3;
};

// What team the entity is on
typedef U8 EntityFaction;
enum {
  EntityFaction_neutral,
  EntityFaction_player,
  EntityFaction_enemy,
};

typedef struct Entity Entity;
struct Entity {
  Entity *next;
  Entity *prev;

  char name_buffer[ENTITY_NAME_SIZE];
  U8 name_len;
  U16 health;
  U16 health_cap;
  U16 health_temp;

  U16 movement_left;
  U16 movement_cap;

  WorldCoord grid_pos;
  Vector2 visual_pos;

  WorldCoordList *path;
  U32 path_index;
  F32 path_ratio;

  EntityFaction faction;
  EffectFlags effects;
  EntityFlags flags;

  U8 action_count;
  U8 action_cap;

  U8 texture; // index into texture buffer
  F32 h_flip;
  U8 animation_state;
};


typedef struct EntityList EntityList;
struct EntityList {
  Entity *first;
  Entity *last;
  Entity *free_list;
  U64 count;
};



// for loop entity iterator
#define EachEntity(it, first) Entity *it = (first); it != NULL; it = it->next 
#define ForEachEntity(it, first) Entity *it = (first); for (Entity *__next__ = it->next; it != NULL; it = __next__, __next__ = it->next)


Entity *EntityAlloc(Arena *arena, EntityList *list, const char *name);
Entity *EntityFindByWorldCoord(EntityList *list, WorldCoord coord);


#endif // ENTITY_H