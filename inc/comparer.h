#ifndef COMPARER_H
#define COMPARER_H

#include <diagnostic.h>
#include <oop.h>
#include <exception.h>

typedef int (*Comparer)(const void *against, const void *reference);

Comparer comparer(const Type *type);
Comparer key_comparer(const Type *type);

#endif