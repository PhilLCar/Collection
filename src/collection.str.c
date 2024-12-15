#include <collection.str.h>

#ifdef COLLECTION_STR_H

#define TYPENAME String

typedef enum {
  FORMAT_ANY,
  FORMAT_FLOAT,
  FORMAT_PTR
} Format;

/******************************************************************************/
int _which_format(const char *format) {
  // This is necessary because floats are stored in XMM registers, so casting is necessary
  // everything else is using normal registers

  char f = format[strlen(format) - 1];

  const char floats[] = {
    'f', 'e', 'E', 'g', 'G'
  };

  const char ptrs[] = {
    'p', 'O'
  };

  for (int i = 0; i < sizeof(floats); i++) {
    if (f == floats[i]) return FORMAT_FLOAT;
  }

  for (int i = 0; i < sizeof(ptrs); i++) {
    if (f == ptrs[i]) return FORMAT_PTR;
  }

  return FORMAT_ANY;
}

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(Split)(const char *other)
{ 
  ObjectArray *array = NEW (ObjectArray) (TYPEOF (String));

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

#define TYPENAME Array

////////////////////////////////////////////////////////////////////////////////
String *CONST (ToStringFormat) (const char *format)
{
  String *result = NEW (String) ("[");

  for (int i = 0; i < this->size; i++) {
    String *item;

    // TODO: Not entirely sure this is the best approach
    switch (_which_format(format)) {
      case FORMAT_FLOAT:
        item = String_Format(format, *(double*)Array_At(this, i));
        break;
      case FORMAT_PTR:
        item = String_Format(format, Array_At(this, i));
        break;
      case FORMAT_ANY:
      default:
        item = String_Format(format, Array_AtDeref(this, i));
        break;
    }
    
    String_Concat(result, String_Format(i == this->size - 1 ? " %Of " : " %Of,", item));
  }

  String_Cat(result, "]");

  return result;
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

////////////////////////////////////////////////////////////////////////////////
String *CONST (ToString) ()
{
  String *result = NEW (String) ("[");

  for (int i = 0; i < BASE(0)->size; i++) {
    String *item = String_ToStringType(Array_At(BASE(0), i), this->type);

    String_Concat(result, String_Format(i == BASE(0)->size - 1 ? " %Of " : " %Of,", item));
  }

  String_Cat(result, "]");

  return result;
}

#undef TYPENAME

#define TYPENAME Pair


////////////////////////////////////////////////////////////////////////////////
String *CONST (ToString) ()
{
  return String_Format("( %Of : %Of )", 
      String_ToString(this->first),
      String_ToString(this->second)
    );
}

#undef TYPENAME

#define TYPENAME List

/******************************************************************************/
String *CONST (toString) (const char *format)
{
  if (List_Empty(this)) {
    return NEW (String) (")");
  } else if (List_Empty(BASE(0)->second)) {
    return String_Format(" %Of )", String_ToString(BASE(0)->first));
  }
  
  return String_Concat(
    String_Format(" %Of,", String_ToString(BASE(0)->first)),
    List_toString(BASE(0)->second, format));
}

////////////////////////////////////////////////////////////////////////////////
String *CONST (ToString) (const char *format)
{
  return String_Concat(NEW (String) ("("), List_toString(this, format));
}

#undef TYPENAME

#endif