#if defined(STR_H) || defined(COLLECTION) || 1
// TODO: Eventually add a preprocessor variable with the name of the lib

#ifndef COLLECTION_STR_H
#define COLLECTION_STR_H

#include <diagnostic.h>
#include <oop.h>
#include <array.h>
#include <objectarray.h>
#include <pair.h>
#include <map.h>
#include <list.h>
#include <str.h>

#define TYPENAME String

ObjectArray *_(Split)(const char *other);

#undef TYPENAME

#define TYPENAME Array

String *CONST (ToString) (const char *format) VIRTUAL (ToString);

#undef TYPENAME

#define TYPENAME ObjectArray

String *_(Join)(const char *link);

FOREIGN_VIRTUAL(ToString, Array_ToString);

#undef TYPENAME

#define TYPENAME Pair

String *CONST (ToString) (const char *format) VIRTUAL (ToString);

#undef TYPENAME

#define TYPENAME Map

FOREIGN_VIRTUAL(ToString, Array_ToString);

#undef TYPENAME

#define TYPENAME List

String *CONST (ToString) (const char *format) VIRTUAL (ToString);

#undef TYPENAME

#endif

#endif