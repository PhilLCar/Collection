#ifndef SET_H
#define SET_H

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <objectarray.h>

#define TYPENAME Set

OBJECT (Type type, Comparer compare) INHERIT (ObjectArray)
  Comparer compare;
END(NATIVE_TYPE(void*), NULL);

void *_(add)(void *data);

#endif