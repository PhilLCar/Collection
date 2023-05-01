#ifndef MAP_H
#define MAP_H

// C
#include <string.h>
#include <stdlib.h>

// CUT
#include <pair.h>
#include <array.h>
#include <oop.h>

#define TYPENAME Map

OBJECT (size_t, size_t) INHERIT (Array)
  size_t size_first;
  size_t size_second;
END_OBJECT;

void *_(atkey)(void *key) ALIAS (atkey);

void *_(vatkey)(void *key) ALIAS (vatkey);

void *_(setkey)(void *key) ALIAS (setkey);

#undef TYPENAME
#endif