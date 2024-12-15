#ifndef LIST_H
#define LIST_H

// C
#include <stdarg.h>

// CUT
#include <oop.h>

#include "pair.h"
#include "comparer.h"

#define TYPENAME List

OBJECT () INHERIT (Pair)
END_OBJECT();

void   *CONST (Head)();
void   *CONST (HeadDeref)();
void   *CONST (Next)();
int     CONST (Empty)();

void   *CONST (At)(int index);
void   *CONST (AtDeref)(int index);
int     CONST (Size)();
void   *CONST (Contains)(const void *element);
void   *CONST (ContainsKey)(const void *key);

List   *CONST (Push)(void *element);
List   *CONST (PushValue)(const Type *type, void *element);

List *STATIC (Fill)(const Type *type, int number, void *elements[number]);

List *_(Pop)(void **object);

List *_(Add)(void *element);
List *_(AddValue)(const Type *type, void *element);

void  _(Remove)(void **object);

List *_(Set)(int index, void *element);
List *_(SetValue)(int index, const Type *type, void *element);

List *_(Insert)(int index, void *element);
List *_(InsertValue)(int index, const Type *type, void *element);

void  _(RemoveAt)(int index, void **object);

void _(Merge)(List *other);

#undef TYPENAME
#endif