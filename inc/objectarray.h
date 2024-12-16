#ifndef OBJECT_ARRAY_H
#define OBJECT_ARRAY_H

// C
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#include "array.h"
#include "comparer.h"

#define TYPENAME ObjectArray

OBJECT (const Type *type) INHERIT (Array)
  const Type *type;
  void       *buffer;
END_OBJECT(TYPEOF (void*));

// Fills the array with values
ObjectArray *STATIC (Fill)(const Type *type, int number, void *elements[number]);

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
void  *_(Remove)(int get);

// Removes a range of elements starting at index start
int    _(RemoveRange)(int start, int range);

// Clears the array
void   _(Clear)();

// Returns a copy of the object with type information present
void  *CONST (At)(int index);

// Returns a pointer to the element targeted if present
void  *CONST (Contains)(const void *reference);

// Returns a pointer to the element that contains the targeted base if present
void  *CONST (ContainsKey)(const void *reference);

// Returns the index of the element targeted
int    CONST (IndexOf)(const void *reference);

// Returns the index of the element that contains the targeted base
int    CONST (IndexOfKey)(const void *reference);

// Returns the iterator associated with the array
void _(Iterator)(Iterator *iterator) VIRTUAL (Iterator);

#undef TYPENAME
#endif
