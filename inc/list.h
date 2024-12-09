#ifndef LIST_H
#define LIST_H

// C
#include <stdarg.h>

// CUT
#include <oop.h>
#include <pair.h>
#include <objectarray.h>

#define TYPENAME List

OBJECT () INHERIT (Pair)
  int object;
END_OBJECT();

void *CONST (Head)();
List *CONST (Next)();

int   CONST (Empty)();

void *CONST (At)(int index);
int   CONST (Size)();
void *CONST (In)(const void *element, Comparer compare);

List *CONST (Push)(const void *element, int object);

List *_(Fill)(int objects, ...);

List *_(Pop)(void **object);

List *_(Add)(const void *element);
void  _(Remove)(void **object);

List *_(Set)(int index, const void *element);
List *_(Insert)(int index, const void *element);
void  _(RemoveAt)(int index, void **object);

void _(Merge)(List *other);

#undef TYPENAME
#endif