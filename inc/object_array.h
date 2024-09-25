#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

// C
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <diagnostic.h>
#include <oop.h>

#include "array.h"

#define TYPENAME ObjectArray

OBJECT (size_t element_size) INHERIT (Array)
END_OBJECT;

// Gets the contained objects' name
const char  *_(object)(int index);

// Fills the array with values
ObjectArray *_(fill)(...);

// Pushes a new element onto the array
void        *_(push)(void *element);

// Pops the last element of the array and frees it
void         _(pop)();

// Removes a range of elements starting at index start
int          _(remrange)(int start, int range);

// Removes the element at index and returns a pointer to it
void         _(rem)(int index);

// Clears the array
void         _(clear)();

// Sets the element at index
void         _(set)(int index, void *element);

// Inserts an element at index
void         _(insert)(int index, void *element);

#undef TYPENAME
#endif
