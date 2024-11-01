#include <collection.str.h>

#define TYPENAME String

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(Split)(const char *other)
{ 
  ObjectArray *array = NEW (ObjectArray) (OBJECT_TYPE(String));

  for (int end = String_Cnt(this, other), len = strlen(other); 
       end >= 0; 
       end = String_Cnt(this, other)) {
    ObjectArray_Push(array, String_SubString(NEW (String) (this->base), 0, end));
    String_SubString(this, end + len, 0);
  }

  ObjectArray_Push(array, this);

  return array;
}
