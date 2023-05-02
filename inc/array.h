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

Array *_(fill)(...);
Array *_(fillo)(...);
int    _(resize)(int newSize);
int    _(recap)(int newCap);
void   _(push)(void *element);
void   _(pusho)(void *element);
void  *_(pop)();
void   _(popo)();
void  *_(popp)();
void  *_(at)(int index);
void  *_(atp)(int index);
void   _(delel)(int index);
void   _(delrange)(int start, int range);
void  *_(rem)(int index);
void  *_(remp)(int index);
void  *_(last)();
void  *_(lastp)();
void   _(set)(int index, void *element);
void   _(clear)();
void   _(combine)(Array *other);
void   _(insert)(int index, void *element);
void  *_(in)(void *element);
void  *_(inp)(void *element);
int    _(indexof)(void *element);

#undef TYPENAME
#endif
