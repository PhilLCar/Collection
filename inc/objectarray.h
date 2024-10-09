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

typedef int (*Comparer)(const void *against, const void *reference);

int default_comparer(const void *against, const void *reference);

OBJECT (Type type) INHERIT (Array)
  Type content;
END(NATIVE_TYPE(void*));

// Fills the array with values
ObjectArray *_(fill)(...);

// Sets the size of the array to the desired value. Recaps if necessary.
int    _(resize)(int new_size);

// Pushes a new element onto the array
void  *_(push)(void *element);

// Pops the last element of the array and returns a pointer to it
void  *_(pop)(int get);

// Removes a range of elements starting at index start
int    _(remrange)(int start, int range);

// Removes the element at index and returns a pointer to it
void  *_(rem)(int index, int get);

// Clears the array
void   _(clear)();

// Sets the element at index
void  *_(set)(int index, void *element);

// Inserts an element at index
void  *_(insert)(int index, void *element);

// Combine two arrays and deletes the other
void   _(addrange)(Array *that);

// Returns a pointer to the element targeted if present
void  *_(in)(void *reference, Comparer comparer);

// Returns the index of the element targeted
int    _(indexof)(void *reference, Comparer comparer);

#undef TYPENAME
#endif
