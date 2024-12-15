#include <list.h>

#define TYPENAME List

////////////////////////////////////////////////////////////////////////////////
List *_(Construct)()
{
  return (List*)Pair_Construct(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    Pair_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Empty)()
{
  return !BASE(0)->second;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (At)(int index)
{
  if (index) {
    return List_At(BASE(0)->second, index - 1);
  }

  return BASE(0)->first;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (AtDeref)(int index)
{
  void **at = List_At(this, index);

  return at ? *at : NULL;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Size)()
{
  List *next = BASE(0)->second;
   
  if (next) return 1 + List_Size(next);

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (Contains)(const void *element)
{
  void *head = BASE(0)->first;
  void *next = BASE(0)->second;

  const Type *type = gettype(head);

  if (!next)                              return NULL;
  else if (comparer(type)(head, element)) return head;

  return List_Contains(next, element);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ContainsKey)(const void *element)
{
  void *head = BASE(0)->first;
  void *next = BASE(0)->second;

  const Type *type = gettype(head);

  if (!next)                                  return NULL;
  else if (key_comparer(type)(head, element)) return head;

  return List_ContainsKey(next, element);
}

////////////////////////////////////////////////////////////////////////////////
List *CONST (Push)(void *element)
{
  return List_PushValue(this, gettype(element), element);
}

////////////////////////////////////////////////////////////////////////////////
List *CONST (PushValue)(const Type *type, void *element)
{
  List *list = NEW (List)();

  Pair_SetValueF((Pair*)list, type, element);
  
  list->base.second = (void*)this;

  return list;
}

////////////////////////////////////////////////////////////////////////////////
List *STATIC (Fill)(const Type *type, int number, void *elements[number])
{
  return number
    ? List_PushValue(List_Fill(type, number - 1, elements + 1), type, elements[0])
    : NEW (List) ();
}

void _(alloc)(void** object)
{
  void *obj = BASE(0)->first;

  if (object) {
    *object = obj;
  } else {
    DELETE (obj);
  }

  BASE(0)->first = NULL;
}

////////////////////////////////////////////////////////////////////////////////
List *_(Pop)(void **object)
{
  List *next = BASE(0)->second;

  List_alloc(this, object);
  tfree(this);

  return next;
}

////////////////////////////////////////////////////////////////////////////////
List *_(Add)(void *element)
{
  return List_AddValue(this, gettype(element), element);
}

////////////////////////////////////////////////////////////////////////////////
List *_(AddValue)(const Type *type, void *element)
{
  List *next = BASE(0)->second;

  if (!next) {    
    Pair_SetValueF(BASE(0), type, element);
    BASE(0)->second = NEW (List) ();

    return this;
  } else {
    return List_AddValue(next, type, element);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(Remove)(void **object) 
{
  List *next = BASE(0)->second;

  if (next) {
    if (next->base.second) {
      List_Remove(next, object);
    } else {
      List_alloc(this, object);

      DELETE (BASE(0)->second);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
List *_(Set)(int index, void *element)
{
  return List_SetValue(this, index, gettype(element), element);
}

////////////////////////////////////////////////////////////////////////////////
List *_(SetValue)(int index, const Type *type, void *element)
{
  if (!index) {
    Pair_SetValueF(BASE(0), type, element);

    return this;
  } else {
    return List_SetValue(BASE(0)->second, index - 1, type, element);
  }
}

////////////////////////////////////////////////////////////////////////////////
List *_(Insert)(int index, void *element)
{
  return List_InsertValue(this, index, gettype(element), element);
}

////////////////////////////////////////////////////////////////////////////////
List *_(InsertValue)(int index, const Type *type, void *element)
{
  List *next = BASE(0)->second;

  if (index == 1) {
    List *insert = NEW (List)();

    Pair_SetValueF(&insert->base, type, element);

    BASE(0)->second     = insert;
    insert->base.second = next;

    return this;
  } else if (!index) {
    return List_PushValue(this, type, element);
  } else {
    return List_InsertValue(next, index - 1, type, element);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveAt)(int index, void **object)
{
  List *next = BASE(0)->second;

  if (!index) {
    Pair_SetF(BASE(0), next->base.first);

    BASE(0)->second = next->base.second;

    tfree(next);
  } else {
    List_RemoveAt(next, index - 1, object);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(Merge)(List *other)
{
  List *next = BASE(0)->second;

  if (!next) {
    BASE(0)->first  = other->base.first;
    BASE(0)->second = other->base.second;

    tfree(other);
  } else {
    List_Merge(next, other);
  }
}


#undef TYPENAME