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
END_OBJECT(TYPEOF (void*));

void *_(Add)(void *data);

#undef TYPENAME
#endif