#include <collection.str.h>

#define TYPENAME String

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(split)(String *other)
{ 
  return String_csplit(this, other->base);
}

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(csplit)(const char *other)
{ 
  ObjectArray *array = NEW (ObjectArray) (OBJECT_TYPE(String));

  int len = strlen(other);

  for (
    int start = 0, end = String_ccontains(this, other);
    end >= 0; 
    start = end + len, end = String_ccontains(this, other)) {
    ObjectArray_push(array, String_substring(NEW (String) (this->base), start, end));
  }

  ObjectArray_push(array, this);

  return array;
}
