#include <collection.str.h>

#define TYPENAME String

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(split)(const char *other)
{ 
  ObjectArray *array = NEW (ObjectArray) (OBJECT_TYPE(String));

  int len   = strlen(other);
  int start = 0;

  for (int end = String_cont(this, other); end >= 0; start = end + len, end = String_cont(this, other)) {
    ObjectArray_push(array, String_substr(NEW (String) (this->base), start, end));
  }

  ObjectArray_push(array, String_substr(this, start, 0));

  return array;
}
