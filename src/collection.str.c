#include <collection.str.h>

#ifdef COLLECTION_STR_H

#define TYPENAME String

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(Split)(const char *other)
{ 
  ObjectArray *array = NEW (ObjectArray) (OBJECT_TYPE(String));

  for (int end = String_Cnt(this, other), len = strlen(other); 
       end >= 0; 
       end = String_Cnt(this, other))
  {
    String *excerpt = end 
      ? String_SubString(NEW (String) (this->base), 0, end)
      : NEW (String) ("");

    ObjectArray_Push(array, excerpt);
    String_SubString(this, end + len, 0);
  }

  ObjectArray_Push(array, this);

  return array;
}

#undef TYPENAME

#define TYPENAME ObjectArray

////////////////////////////////////////////////////////////////////////////////
String *_(Join)(const char *link)
{ 
  String *string = NEW (String) ("");

  for (int i = 0; i < this->base.size; i++)
  {
    String_Concat(string, ObjectArray_Remove(this, 1));

    if (i < this->base.size - 1) {
      String_Concat(string, NEW (String) (link));
    }
  }

  return string;
}

#undef TYPENAME

#endif