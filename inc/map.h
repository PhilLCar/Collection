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

// In maps, efficient comparison is essential
// this is why we compute the comparer in the constructor here

OBJECT (const Type *key) INHERIT (ObjectArray)
  const Type *key;
  Comparer    comparer;
  Comparer    keyComparer;
END_OBJECT(TYPEOF (const char*));

Pair *_(Set)(void *key, void *value);
Pair *_(SetValue)(void *key, const Type *valueType, void *value);
void  _(Remove)(const void *key);
void  _(RemoveKey)(const void *key);

Pair *CONST (At)(const void *key);
Pair *CONST (AtKey)(const void *key);
void *CONST (ValueAt)(const void *key);
void *CONST (ValueAtKey)(const void *key);
void *CONST (ValueAtDeref)(const void *key);
void *CONST (ValueAtKeyDeref)(const void *key);

#undef TYPENAME
#endif