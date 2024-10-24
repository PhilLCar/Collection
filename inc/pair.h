#ifndef PAIR_H
#define PAIR_H

#include <diagnostic.h>
#include <oop.h>

#define TYPENAME Pair

typedef struct _pair_member {
  void *object;
  Type  type;
} PairMember;

OBJECT (Type first, Type second)
  PairMember first;
  PairMember second;
END(NATIVE_TYPE(void*), NATIVE_TYPE(void*));

Pair *STATIC (from)(void *first, void *second);
void *STATIC (set)(PairMember *member, void *element);

void *_(setf)(void *element);
void *_(sets)(void *element);

void *_(fptr)();
void *_(sptr)();

#undef TYPENAME
#endif