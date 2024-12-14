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
String *CONST (ToString) (const char *format)
{
  String *result = NEW (String) ("[");

  for (int i = 0; i < this->size; i++) {
    String *item;

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

    if (i == this->size - 1) {
      item = String_Format(" %Of ", item);
    } else {
      item = String_Format(" %Of,", item);
    }

    String_Concat(result, item);
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

#undef TYPENAME

#define TYPENAME Pair

/******************************************************************************/
String *STATIC (toStringMember)(const PairMember *member, const char *format)
{
  String *item;

  switch (_which_format(format)) {
    case FORMAT_FLOAT:
      item = String_Format(format, *(double*)member->object);
      break;
    case FORMAT_PTR:
      item = String_Format(format, member->object);
      break;
    case FORMAT_ANY:
    default:
      item = String_Format(format, member->object ? *(void**)member->object : NULL);
      break;
  }

  return item;
}

////////////////////////////////////////////////////////////////////////////////
String *CONST (ToString) (const char *format)
{
  String *result = NEW (String) ("( ");

  char *first = malloc(strlen(format) + 1);

  strcpy(first, format);

  char *second = strstr(first, ":");

  *(second++) = 0;

  String_Concat(result, Pair_toStringMember(&this->first, first));
  String_Cat(result, " : ");
  String_Concat(result, Pair_toStringMember(&this->second, second));
  String_Cat(result, " )");

  free(first);

  return result;
}

#undef TYPENAME

#define TYPENAME List

/******************************************************************************/
String *CONST (toString) (const char *format)
{
  if (List_Empty(this)) {
    return NEW (String) (")");
  } else if (List_Empty(List_Next(this))) {
    return String_Format(" %O )", Pair_toStringMember(&BASE(0)->first, format));
  }
  
  return String_Concat(
    String_Format(" %Of,", Pair_toStringMember(&BASE(0)->first, format)),
    List_toString(List_Next(this), format));
}

////////////////////////////////////////////////////////////////////////////////
String *CONST (ToString) (const char *format)
{
  return String_Concat(NEW (String) ("("), List_toString(this, format));
}

#undef TYPENAME

#endif