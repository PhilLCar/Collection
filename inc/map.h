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

OBJECT (const Type *key, const Type *value, Comparer compare) INHERIT (ObjectArray)
  const Type *key;
  const Type *value;
  Comparer    compare;
END_OBJECT(TYPEOF (NATIVE(void*)), TYPEOF (NATIVE(const char *)), (Comparer)strcmp);

Pair *_(Set)(void *key, void *value);
void  _(Remove)(const void *key);

Pair *CONST (At)(const void *key);
void *CONST (ValueAt)(const void *key);
void *CONST (ValueAtDeref)(const void *key);

#undef TYPENAME
#endif