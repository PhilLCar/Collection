#ifndef ARRAY_H
#define ARRAY_H

// C
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <diagnostic.h>
#include <oop.h>

#define TYPENAME Array

OBJECT (size_t element_size) INHERIT (void*)
  int     size;
  int     capacity;
  size_t  element_size;
END_OBJECT;

// Fills the array with values
TYPENAME *_(fill)(...);

// Sets the capacity of the array to the desired value. Truncates if necessary.
int    _(recap)(int newCap);

// Sets the size of the array to the desired value. Recaps if necessary.
int    _(resize)(int newSize);

// Pushes a new element onto the array
void   _(push)(void *element);

// Pops the last element of the array and returns a pointer to it
void  *_(pop)();

// Pops the last element and returns it
void  *_(popp)();

// Returns a pointer to the element at index
void  *_(at)(int index);

// Returns the element at index
void  *_(atp)(int index);

// Returns a pointer to the last element of the array
void  *_(last)();

// Returns the last element from the array
void  *_(lastp)();

// Removes a range of elements starting at index start
int    _(remrange)(int start, int range);

// Removes the element at index and returns a pointer to it
void  *_(rem)(int index);

// Removes the element at index and returns it
void  *_(remp)(int index);

// Clears the array
void   _(clear)();

// Sets the element at index
void   _(set)(int index, void *element);

// Inserts an element at index
void   _(insert)(int index, void *element);

// Combine two arrays and deletes the other
void   _(combine)(Array *other);

// Returns a pointer to the element targeted if present
void  *_(in)(void *element);

// Returns the element targeted if present
void  *_(inp)(void *element);

// Returns the index of the element targeted
int    _(indexof)(void *element);

#undef TYPENAME
#endif
