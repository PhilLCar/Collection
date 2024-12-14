#ifndef ARRAY_H
#define ARRAY_H

// C
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// CUT
#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#define TYPENAME Array

OBJECT (size_t element_size) BASED (void*)
  int     size;
  int     capacity;
  size_t  element_size;
END_OBJECT(sizeof(void*));

// Fills the array with values
Array *STATIC (Fill)(size_t element_size, int number, const void *elements);

// Sets the capacity of the array to the desired value. Truncates if necessary.
int    _(Recap)(int newCap);

// Sets the size of the array to the desired value. Recaps if necessary.
int    _(Resize)(int newSize);

// Inserts an element at index
void  *_(Insert)(int index, const void *element);

// Pushes a new element onto the array (last)
void  *_(Push)(const void *element);

// Adds a new element in the array (first)
void  *_(Add)(const void *element);

// Combine two arrays (deletes the other)
void   _(Merge)(Array *that);

// Swaps 2 elements of the array specified by their indexes
int    _(Swap)(int index1, int index2);

// Sets the element at index
void  *_(Set)(int index, const void *element);

// Removes the element at index and returns a pointer to it
void  *_(RemoveAt)(int index);

// Removes the element at index and returns it
void  *_(RemoveAtDeref)(int index);

// Pops the last element of the array and returns a pointer to it
void  *_(Pop)();

// Pops the last element and returns it
void  *_(PopDeref)();

// Removes the element at 0 and returns a pointer to it
void  *_(Remove)();

// Removes the element at 0 and returns it
void  *_(RemoveDeref)();

// Removes a range of elements starting at index start
int    _(RemoveRange)(int start, int range);

// Clears the array
void   _(Clear)();

// Checks if the index is within the array
int    CONST (Index)(int *index);

// Returns a pointer to the element at index
void  *CONST (At)(int index);

// Returns the element at index
void  *CONST (AtDeref)(int index);

// Returns a pointer to the last element of the array
void  *CONST (Last)();

// Returns the last element from the array
void  *CONST (LastDeref)();

// Returns a pointer to the element targeted if present
void  *CONST (Contains)(const void *element);

// Returns the element targeted if present
void  *CONST (ContainsDeref)(const void *element);

// Returns the index of the element targeted
int    CONST (IndexOf)(const void *element);

#undef TYPENAME
#endif
