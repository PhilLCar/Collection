#ifndef COLLECTION_STR_H
#define COLLECTION_STR_H

#include <diagnostic.h>
#include <oop.h>
#include <objectarray.h>
#include <str.h>

#define TYPENAME String

ObjectArray *_(split)(const char *other);


#undef TYPENAME
#endif