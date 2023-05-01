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

OBJECT (size_t) INHERIT (void*)
  int     size;
  int     capacity;
  size_t  element_size;
  int     objects_inside;
END_OBJECT;

// Fills an array with the elements "..."
Array *_(fill)(...) ALIAS (fill);

Array *_(fillo)(...) ALIAS (fillo);

// Resize the <array> to the specified <newSize>
// RETURNS 1 if successful, 0 otherwise
int    _(resize)(int newSize) ALIAS (resize);

// Recap the <array> to the specified <newCap>
// RETURNS 1 if successful, 0 otherwise
int    _(recap)(int newCap) ALIAS (recap);

// Copies a new <element> onto the <array>, (keeps reference)
void   _(push)(void *element) ALIAS (push);

// Copies a new <element> onto the <array>, (deletes reference)
void   _(pusho)(void *element) ALIAS (pusho);

// Pops an element from the <array>
// RETURNS a pointer to it
void  *_(pop)() ALIAS (pop);

void   _(popo)() ALIAS(popo);

// Pops an element from the <array>
// RETURNS the pointer that it is
void  *_(popp)() ALIAS (popp);

// RETURNS a pointer to the element at the specified <index>
void  *_(at)(int index) ALIAS (at);

// RETURNS the pointer that is the element at the specified <index>
void  *_(atp)(int index) ALIAS (atp);

void   _(delel)(int index) ALIAS (delel);
void   _(delrange)(int start, int range) ALIAS (delrange);

// Removes the element at <index>
// RETURNS a pointer to a copy of it
void  *_(rem)(int index) ALIAS (rem);

// Removes the element at <index>
// RETURNS the pointer that was removed
void  *_(remp)(int index) ALIAS (remp);

// RETURNS a pointer to the last element of the <array>
void  *_(last)() ALIAS (last);

// RETURNS the last element of the <array>
void  *_(lastp)() ALIAS (lastp);

// Sets the <element> at the specified <index>
void   _(set)(int index, void *element) ALIAS (set);

// Clear all data in the <array>
void   _(clear)() ALIAS (clear);

// Adds the <delete> array to the back of the <persist> one
void   _(combine)(Array *other) ALIAS (combine);

// Insert an <element> at the specified <index>
void   _(insert)(int index, void *element) ALIAS (insert);

// RETURNS a pointer to the <element> if present, NULL otherwise
void  *_(in)(void *element) ALIAS (in);

// RETURNS a pointer to the <element> if present, NULL otherwise
void  *_(inp)(void *element) ALIAS (inp);

// RETURNS the index of the <element> if present, -1 otherwise
int    _(indexof)(void *element) ALIAS (indexof);

#undef TYPENAME
#endif
