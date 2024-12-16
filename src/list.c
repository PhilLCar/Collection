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
void *CONST (Head)()
{
  return BASE(0)->first;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (HeadDeref)()
{
  void **ptr = List_Head(this);

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (Next)()
{
  return BASE(0)->second;
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

  if (!next)                                       return NULL;
  else if (comparer(gettype(head))(head, element)) return head;

  return List_Contains(next, element);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ContainsKey)(const void *element)
{
  void *head = BASE(0)->first;
  void *next = BASE(0)->second;

  if (!next)                                           return NULL;
  else if (key_comparer(gettype(head))(head, element)) return head;

  return List_ContainsKey(next, element);
}

////////////////////////////////////////////////////////////////////////////////
List *CONST (Push)(void *element)
{
  return List_PushValue(this, NULL, element);
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
List *_(Fill)(...)
{
  va_list  argptr;
  void    *current;
  List    *last = this;

  va_start(argptr, this);

  while ((current = va_arg(argptr, void*))) {
    last = List_Add(last, current);  
  }

  va_end(argptr);

  return this;
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
  return List_AddValue(this, NULL, element);
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
  return List_SetValue(this, index, NULL, element);
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
  return List_InsertValue(this, index, NULL, element);
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

/******************************************************************************/
int STATIC(done)(Iterator *iterator)
{
  return List_Empty(iterator->collection);
}

/******************************************************************************/
void STATIC(next)(Iterator *iterator)
{
  iterator->collection = List_Next(iterator->collection);
  iterator->base       = List_Head(iterator->collection);
}

/******************************************************************************/
int STATIC(count)(Iterator *iterator)
{
  return List_Size(iterator->collection);
}

////////////////////////////////////////////////////////////////////////////////
void _(Iterator)(Iterator *iterator)
{
  iterator->base       = List_Head(this);
  iterator->collection = this;
  iterator->env        = NULL;
  iterator->done       = List_done;
  iterator->next       = List_next;
  iterator->end        = NULL;
  iterator->count      = List_count;
}

#undef TYPENAME