#ifndef MAPSET_H
#define MAPSET_H

// C
#include <string.h>
#include <stdlib.h>

// CUT
#include <diagnostic.h>
#include <oop.h>

#include "set.h"
#include "keyval.h"

#define TYPENAME MapSet

// In maps, efficient comparison is essential
// this is why we compute the comparer in the constructor here

OBJECT (const Type *key) INHERIT (Set)
  KeyValEnvironment env;
END_OBJECT(TYPEOF (const char*));

KeyVal *_(Set)(void *key, void *value);
KeyVal *_(SetKey)(const void *key, void *value);
KeyVal *_(SetValue)(void *key, const Type *valueType, void *value);
KeyVal *_(SetKeyValue)(const void *key, const Type *valueType, void *value);
void    _(Remove)(const void *key);
void    _(RemoveKey)(const void *key);

KeyVal *CONST (At)(const void *key);
KeyVal *CONST (AtKey)(const void *key);
void   *CONST (ValueAt)(const void *key);
void   *CONST (ValueAtKey)(const void *key);
void   *CONST (ValueAtDeref)(const void *key);
void   *CONST (ValueAtKeyDeref)(const void *key);

#undef TYPENAME
#endif