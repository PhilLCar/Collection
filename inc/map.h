#ifndef MAP_H
#define MAP_H

// C
#include <string.h>
#include <stdlib.h>

// CUT
#include <diagnostic.h>
#include <oop.h>

#include "object_array.h"
#include "pair.h"

#define TYPENAME Map

typedef int (*Comparer)(void*, void*);

OBJECT (size_t key_size, size_t value_size, Comparer comparer) INHERIT (ObjectArray)
  size_t key_size;
  size_t value_size;  
  Comparer comparer;
END_OBJECT;

Pair *_(atkey)(void *key);
void *_(vatkey)(void *key);
Pair *_(setkey)(void *key, void *value);
void  _(remkey)(void *key);

#undef TYPENAME
#endif