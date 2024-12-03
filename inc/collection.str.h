#if defined(STR_H) || 1
// TODO: Eventually add a preprocessor variable with the name of the lib

#ifndef COLLECTION_STR_H
#define COLLECTION_STR_H

#include <diagnostic.h>
#include <oop.h>
#include <objectarray.h>
#include <str.h>

#define TYPENAME String

ObjectArray *_(Split)(const char *other);

#undef TYPENAME

#define TYPENAME ObjectArray

String *_(Join)(const char *link);

#undef TYPENAME

#endif

#endif