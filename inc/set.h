#ifndef SET_H
#define SET_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <exception.h>
#include <objectarray.h>

#define TYPENAME Set

OBJECT (const Type *type, Comparer compare) INHERIT (ObjectArray)
  Comparer compare;
END_OBJECT(TYPEOF (NATIVE(void*)), NULL);

void *_(Add)(void *data);

#undef TYPENAME
#endif