#ifndef ITERATOR_H
#define ITERATOR_H

#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

#define TYPENAME Iterator

OBJECT (void *collection) INHERIT (void*)
  void   *collection;
  void   *env;
  int   (*done)(Iterator *this);
  void  (*next)(Iterator *this);
  void  (*end)(Iterator *this);
  int   (*count)(Iterator *this);
END_OBJECT(NULL);

int  done(Iterator *e);
void next(Iterator *e);
int  count(Iterator *e);

#undef TYPENAME

#endif