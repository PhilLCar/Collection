#ifndef MAP_H
#define MAP_H

// C
#include <string.h>
#include <stdlib.h>

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <str.h>

#include "array.h"
#include "pair.h"

#define TYPENAME Map

OBJECT (size_t, size_t) INHERIT (Array)
  size_t size_first;
  size_t size_second;
END_OBJECT;

void *_(atkey)(void *key);
void *_(vatkey)(void *key);
void *_(setkey)(void *key);

#undef TYPENAME
#endif