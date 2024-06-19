#ifndef PAIR_H
#define PAIR_H

#include <oop.h>
#include <diagnostic.h>

#define TYPENAME Pair

OBJECT (size_t first_size, size_t second_size)
  size_t  first_size;
  void   *first;
  size_t  second_size;
  void   *second;
END_OBJECT;

const char *_(fobject)();
const char *_(sobject)();

void _(frem)();
void _(srem)();

void *_(fset)(void *element);
void *_(sset)(void *element);

#undef TYPENAME
#endif