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
ObjectArray *_(Fill)(...);

// Sets the size of the array to the desired value. Recaps if necessary.
int    _(Resize)(int newSize);

// Inserts an element at index
void  *_(Insert)(int index, void *element);

// Pushes a new element onto the array
void  *_(Push)(void *element);

// Pushes a new element onto the array
void  *_(Add)(void *element);

// Sets the element at index
void  *_(Set)(int index, void *element);

// Removes the element at index and returns a pointer to it
void  *_(RemoveAt)(int index, int get);

// Pops the last element of the array and returns a pointer to it
void  *_(Pop)(int get);

// Removes the element at index and returns a pointer to it
void  *_(Remove)(int index, int get);

// Removes a range of elements starting at index start
int    _(RemoveRange)(int start, int range);

// Clears the array
void   _(Clear)();

// Returns a pointer to the element targeted if present
void  *CONST (In)(const void *reference, Comparer comparer);

// Returns the index of the element targeted
int    CONST (IndexOf)(const void *reference, Comparer comparer);

#undef TYPENAME
#endif
