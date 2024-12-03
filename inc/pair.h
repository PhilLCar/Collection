#ifndef PAIR_H
#define PAIR_H

#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#define TYPENAME Pair

typedef struct _pair_member {
  void       *object;
  const Type *type;
} PairMember;

OBJECT (const Type *first, const Type *second) NOBASE
  PairMember first;
  PairMember second;
END_OBJECT(TYPEOF (NATIVE(void*)), TYPEOF (NATIVE(void*)));

Pair *STATIC (From)(void *first, void *second);
void *STATIC (Set)(PairMember *member, void *element);

void *_(SetF)(void *element);
void *_(SetS)(void *element);

void *CONST (DerefF)();
void *CONST (DerefS)();

#undef TYPENAME
#endif