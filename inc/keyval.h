#ifndef KEYVAL_H
#define KEYVAL_H

#include <diagnostic.h>
#include <oop.h>

#include "pair.h"
#include "set.h"

#define TYPENAME KeyVal

typedef int (*EnvComparer)(void *against, void *reference, void *envptr);

typedef struct {
  Comparer  comparer;
  Comparer  keyComparer;
  void     *pointer; // Use this to retrieve any information from the comparer
} KeyValEnvironment;

OBJECT (KeyValEnvironment *env) INHERIT (Pair)
  KeyValEnvironment *env;
END_OBJECT(NULL);

int _(Comparer)   (KeyVal *reference) VIRTUAL (Comparer);
int _(KeyComparer)(KeyVal *reference) VIRTUAL (KeyComparer);

int _(EnvComparer)   (KeyVal *reference);
int _(EnvKeyComparer)(KeyVal *reference);

#undef TYPENAME
#endif