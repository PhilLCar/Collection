#ifndef COLLECTION_STR_H
#define COLLECTION_STR_H

#include <objectarray.h>
#include <str.h>
#include <oop.h>

#define TYPENAME String

ObjectArray *_(split)(String *other);
ObjectArray *_(csplit)(const char *other);


#undef TYPENAME
#endif