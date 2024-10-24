#ifndef MAP_H
#define MAP_H

// C
#include <string.h>
#include <stdlib.h>

// CUT
#include <diagnostic.h>
#include <oop.h>

#include "objectarray.h"
#include "pair.h"

#define TYPENAME Map


static int streq(const char *a, const char *b) {
  return !strcmp(a, b);
}

OBJECT (Type key, Type value, Comparer compare) INHERIT (ObjectArray)
  Type     key;
  Type     value;
  Comparer compare;
END(NATIVE_TYPE(const char *), NATIVE_TYPE(void*), (Comparer)streq);

Pair *_(atkey)(const void *key);
void *_(vatkey)(const void *key);
void *_(vatkeyptr)(const void *key);
Pair *_(setkey)(void *key, void *value);
void  _(remkey)(const void *key);

#undef TYPENAME
#endif