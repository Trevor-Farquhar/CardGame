#include "world.h"
#include "agent.h"
#include <raymath.h>
// Coordinate Type Conversion Functions 
Vector2 Vector2FromWorldIndex(World *world, U32 index) {
  return (Vector2){
    .x = index % world->width,
    .y = index / world->width,
  };
}

Vector2 Vector2FromWorldCoord(WorldCoord coord) {
  return (Vector2) {
    .x = (F32)coord.x,
    .y = (F32)coord.y,
  };
}

WorldCoord WorldCoordFromVector2(Vector2 v) {
  return (WorldCoord){v.x, v.y};
}

WorldCoord WorldCoordFromIndex(World *world, U32 index) {
  return (WorldCoord){
    .x = index % world->width,
    .y = index / world->width,
  };
}

U32 WorldIndexFromVector2(World *world, Vector2 v) {
  return (U32)v.x + (U32)v.y * world->width;
}

U32 WorldIndexFromWorldCoord(World *world, WorldCoord coord) {
  return coord.x + coord.y * world->width;
}


World WorldInit(Arena *arena, U32 width, U32 height) {
  return (World) {
    .width = width,
    .height = height,
    .tiles = ArenaPush(arena, sizeof(Tile) * width * height),
    .camera = (Camera2D) {
      .zoom = 2 * width,
    },
    .entity_grid = ArenaPush(arena, sizeof(Entity*) * width * height),
    .entities = ArenaPush(arena, sizeof(EntityList)),
  };
}

void WorldLoad(World *world, Arena* arena, const char *filepath) {
  // TODO: implement this function
}

void WorldSave(World *world, const char *filepath) {
  // TODO: implement this function
}

void WorldDraw(World *world) {
  BeginMode2D(world->camera);
  {
    for (U32 i = 0; i < world->width * world->height; i++) {
      Vector2 tile_pos = Vector2FromWorldIndex(world, i);

      Color color;

      switch (world->tiles[i]) {
        case Tile_wall: color = DARKGRAY; break;
        default: color = BLACK; break;
      }

      DrawRectangleV(tile_pos, (Vector2){1,1}, color);

    }


    for (EachEntity(entity, world->entities->first)) {
      DrawRectangleV(entity->visual_pos, Vector2One(), GREEN);
    }
  }
  EndMode2D();
}

void WorldUpdateFrame(World *world, Arena *temp_arena) {
  // TODO: implement this function
  Vector2 mouse_pos = GetMousePosition();
  Vector2 mouse_world_pos = GetScreenToWorld2D(mouse_pos, world->camera);
  WorldCoord mouse_world_coord = WorldCoordFromVector2(mouse_world_pos);
  // if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
  //   const U32 index = WorldIndexFromVector2(world, mouse_world_pos);
  //   world->tiles[index] = Tile_wall;
  // }

  // if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
  //   const U32 index = WorldIndexFromVector2(world, mouse_world_pos);
  //   world->tiles[index] = Tile_void;
  // }

  BeginMode2D(world->camera);

  U64 arena_size_before = temp_arena->pos;
  WorldCoordList *path_list = FindPath(world, temp_arena, (WorldCoord){0,0}, mouse_world_coord, 0);
  U64 arena_size_afer = temp_arena->pos;

  if (path_list == NULL) 
    TraceLog(LOG_DEBUG, "What happened here");


  WorldCoordListDraw(world, path_list);

  // TraceLog(LOG_INFO, "Bytes Per Path Find: %lu", arena_size_afer - arena_size_before);
  EndMode2D();

  

}

void WorldUpdateWorld(World *world) {
  // TODO: implement this function
}
