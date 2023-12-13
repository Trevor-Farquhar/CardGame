/* 
  This is not a actual src file but a style guide, introduction to the 
  codebase, and mini c tutorial.
*/

/*
  Struct Declaration

  Lets start off with struct declaration,

  we make heavy use of linked-lists in this library
  so we want to be able to have pointer members of the
  same type as our struct.

  Why we use linked lists (just a suggestion)
    https://www.rfleury.com/p/in-defense-of-linked-lists 

*/

// This is the correct way to declare a struct
typedef struct EntityExample EntityExample;
struct EntityExample {
  // This style of declarations lets us declare a pointer like this
  EntityExample *next; 
  EntityExample *prev;
  EntityExample *first_child;
  EntityExample *last_child;
  // other stuff goes here
};

// Wrong ways to declare a struct
typedef struct {
  // Declaring structs this way we cannot 
  // get the name of the pointer type
  // because of c out-dated design
  void *next; 
} Entity2;

// This is also bad
typedef struct Entity3 {
  // Declaring it like this makes us use 
  // the struct keyword to get the 
  // correct pointer type.
  struct Entity3 *next;
} Entity3;

/*
  Lets now talk about variable declarations 

  For variable declarations we will use snake_case
*/

// this is how you get our basic types
// This should be included in almost every header file
#include "include/basetypes.h"   
// NOTE: in the src folder include it like this #include <basetypes.h>

// This is correct 
U32 counter = 0; // this is a 32-bit unsigned integer

// This is wrong (this is same as above under hood)
unsigned int counter2 = 0; // we dont use these long ass integer names

// This is right
U64 page_size = 4096; // use of snake case and a unsigned integer when the integer is not negative

// This is wrong
I64 pageSize = 4096; // the integer does not need to be negative and is camelCase

// This is how we declare a struct variable 
EntityExample some_entity = (EntityExample){
  .first_child = NULL,
  .last_child = NULL,
  .next = NULL,
  .prev = NULL,
};

// Or like this 
EntityExample another_entity = { 0 }; // this is how you zero initialize a struct

// Or we can declare like this if we want some
// fields to be initialized to 0 and some to have 
// explicit data

EntityExample yet_another_entity = (EntityExample){.next = &some_entity}; // all other fields will be set to 0 aka NULL

// This is the wrong way 
EntityExample yet_another_entity = {0,0,0,0}; // Bad because we do not know what is being set to what

// Only do this if you know what you are doing
EntityExample some_other_entity; // very bad because its just random memory here

/*
  Functions
   
  Okey lets do how to declare functions 

*/

// example type
typedef struct Vec2 Vec2;
struct Vec2 {
  F32 x;
  F32 y;
};

// If the type is small enough we 
// can just pass by value and 
Vec2 Vec2Add(Vec2 a, Vec2 b) {
  return (Vec2){
    .x = a.x + b.x,
    .y = a.y + b.y,
  };
}

// All function that our written by us
// should be declared in PascalCase
void SomeMain(void) {
  Vec2 a = {10, 10}; // for simple struct types like this this stye is ok
  Vec2 b = {5, 5};

  a = Vec2Add(a, b); // This is like saying a += b

}

/*
  Pointers
*/

void SomeProcedure(void) {
  // Lets always initalize our pointers
  void *some_ptr = NULL;

  // if checking if a ptr has a value go like this 
  if (some_ptr) {
    // do somethings with some_ptr
  }

  // We prefer to use the style above 
  // but if you need to check if a pointer is
  // null you do an explicit comparason
  if (some_ptr == NULL) {
    // do something if the pointer is null
  }
}

// Here is what you do not want to do
void SomeBadProcedure(void) {
  void *some_ptr; // we dont initialize it

  if (some_ptr != NULL) {} // this is ugly

  
}

/*
  Memory Allocation

  For this code base to make things simple 
  we will only* use the arena allocator in
  arena.h in the src folder for dynamic allocation

  We will most likely only have two arena allocators for the
  whole application.

  * some raylib functions use the malloc allocator so we have to be careful
  when using those functions.

  Long Video (conference talk) that has lots of knowledge (would recommend)
    https://vimeo.com/856069590 please watch - maybe 30 minutes a day until completed

  watch this video for a good explaination on Arena Allocators:
    https://www.youtube.com/watch?v=3IAlJSIjvH0

  Really Long Article On Arenas
    https://www.rfleury.com/p/untangling-lifetimes-the-arena-allocator
  
  Quick and Simple Article
    https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
*/

#include "src/arena.h"
#include <string.h> // just examples

// example type
typedef struct String String;
struct String {
  U64 len;
  char *ptr; // for character types use char instead of U8
};

// for declaring String Literals on the stack
String StringLit(char *str) {
  U32 count; // We can leave undefined since its value is declared immiedately after

  // basically strlen in string.h
  for (count = 0; str[count] != '\0'; count++) 
    {}
  
  count = strlen(str); // useless does the same thing

  return (String){
    .len = count,
    .ptr = str,
  };
}

// This function makes an heap allocation and returns a string
// Our equivalent of strcat
String StringCat(Arena *arena, String a, String b) { // Arena are always the first param
  U64 len = a.len + b.len;
  char *ptr = ArenaPush(arena, len);

  memcpy(ptr, a.ptr, a.len);
  memcpy(ptr + a.len, b.ptr, b.len); // some pointer aritmetic 

  return (String){
    .len = len,
    .ptr = ptr,
  };
}

