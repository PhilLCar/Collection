#ifndef PAIR_H
#define PAIR_H

#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#define TYPENAME Pair

OBJECT () NOBASE
  void *first;
  void *second;
END_OBJECT();

Pair *STATIC (From)(void *first, void *second);

void *_(SetF)(void *element);
void *_(SetS)(void *element);

void *_(SetValueF)(const Type *type, void *element);
void *_(SetValueS)(const Type *type, void *element);

void *CONST (FDeref)();
void *CONST (SDeref)();

#undef TYPENAME
#endif