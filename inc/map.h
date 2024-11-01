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

OBJECT (Type key, Type value, Comparer compare) INHERIT (ObjectArray)
  Type     key;
  Type     value;
  Comparer compare;
END_OBJECT(NATIVE_TYPE(const char *), NATIVE_TYPE(void*), (Comparer)strcmp);

Pair *_(Set)(void *key, void *value);
void  _(Remove)(const void *key);

Pair *CONST (At)(const void *key);
void *CONST (ValueAt)(const void *key);
void *CONST (ValueAtDeref)(const void *key);

#undef TYPENAME
#endif