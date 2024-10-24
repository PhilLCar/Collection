#include <collection.str.h>

#define TYPENAME String

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(split)(const char *other)
{ 
  ObjectArray *array = NEW (ObjectArray) (OBJECT_TYPE(String));

  for (int end = String_cont(this, other), len = strlen(other); 
       end >= 0; 
       end = String_cont(this, other)) {
    ObjectArray_push(array, String_substr(NEW (String) (this->base), 0, end));
    String_substr(this, end + len, 0);
  }

  ObjectArray_push(array, this);

  return array;
}
