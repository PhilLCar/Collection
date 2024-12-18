#ifndef SET_H
#define SET_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#include "objectarray.h"

#define TYPENAME Set

OBJECT (const Type *type) INHERIT (ObjectArray)
  Comparer comparer;
  Comparer keyComparer;
END_OBJECT(TYPEOF (void*));

int CONST (Contains)(const void *data);
int CONST (ContainsKey)(const void *data);

void *_(Add)(void *data);
void *_(AddValue)(const Type *type, void *data);
int   _(Remove)(const void *data);
int   _(RemoveKey)(const void *data);
Set  *_(Union)(Set *other);
Set  *_(Intersect)(Set *other);

#undef TYPENAME
#endif