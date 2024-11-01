#ifndef PAIR_H
#define PAIR_H

#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#define TYPENAME Pair

typedef struct _pair_member {
  void *object;
  Type  type;
} PairMember;

OBJECT (Type first, Type second) NOBASE
  PairMember first;
  PairMember second;
END_OBJECT(NATIVE_TYPE(void*), NATIVE_TYPE(void*));

Pair *STATIC (From)(void *first, void *second);
void *STATIC (Set)(PairMember *member, void *element);

void *_(SetF)(void *element);
void *_(SetS)(void *element);

void *CONST (DerefF)();
void *CONST (DerefS)();

#undef TYPENAME
#endif