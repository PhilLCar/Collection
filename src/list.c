#include <list.h>

#define TYPENAME List

////////////////////////////////////////////////////////////////////////////////
List *_(Construct)()
{
  if (Pair_Construct(BASE(0), TYPEOF (NATIVE(void*)), TYPEOF (NATIVE(void*)))) {
    // default
    this->object = 0;
  }
  
  // Empty list
  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    List *next = List_Next(this);
    void *head = List_Head(this);

    DELETE (next);

    if (this->object)
    {
      DELETE (head);
    }

    Pair_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (Head)()
{
  return Pair_FDeref(BASE(0));
}

List *CONST (Next)()
{
  return Pair_SDeref(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Empty)()
{
  return !List_Next(this);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (At)(int index)
{
  if (index) {
    List *next = List_Next(this);

    return List_At(next , index - 1);
  }

  return List_Head(this);
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Size)()
{
  List *next = List_Next(this);
   
  if (next) return 1 + List_Size(next);

  return 0;
}

/******************************************************************************/
Comparer CONST (comparer)()
{
  Comparer comparer = default_comparer;

  if (this->object) {
    comparer = IFNULL(virtual(gettype(List_Head(this)), "Comparer"), default_comparer);
  }

  return comparer;
}

/******************************************************************************/
Comparer CONST (keyComparer)()
{
  Comparer comparer = default_comparer;

  if (this->object) {
    comparer = IFNULL(virtual(gettype(List_Head(this)), "KeyComparer"), default_key_comparer);
  }

  return comparer;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (Contains)(const void *element)
{
  void *head = List_Head(this);
  void *next = List_Next(this);

  if (!next)                           return NULL;
  if (!List_comparer(this)(head, element)) return head;

  return List_Contains(next, element);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ContainsKey)(const void *element)
{
  void *head = List_Head(this);
  void *next = List_Next(this);

  if (!next)                           return NULL;
  if (!List_keyComparer(this)(head, element)) return head;

  return List_Contains(next, element);
}

////////////////////////////////////////////////////////////////////////////////
List *CONST (Push)(const void *element, int object)
{
  List *list = NEW (List)();

  Pair_SetF((Pair*)list, &element);
  Pair_SetS((Pair*)list, &this);

  list->object = object;

  return list;
}

////////////////////////////////////////////////////////////////////////////////
List *_(Fill)(int objects, ...)
{
  void    *arg;
  va_list  args;

  va_start(args, objects);
  while ((arg = va_arg(args, void*))) {
    List_Add(this, arg)->object = objects;
  }
  va_end(args);

  return this;
}

void _(alloc)(void** object)
{
  if (object) {
    *object = List_Head(this);

    memset(BASE(0)->first.object, 0, sizeof(void*));
  } else if (this->object) {
    void *obj = List_Head(this);

    DELETE (obj);
  }
}

////////////////////////////////////////////////////////////////////////////////
List *_(Pop)(void **object)
{
  List *next = List_Next(this);

  List_alloc(this, object);

  Pair_Destruct(BASE(0));
  tfree(this);

  return next;
}

////////////////////////////////////////////////////////////////////////////////
List *_(Add)(const void *element)
{
  List *next = List_Next(this);

  if (!next) {
    next = NEW (List)();

    Pair_SetF(BASE(0), &element);
    Pair_SetS(BASE(0), &next);

    return this;
  } else {
    return List_Add(next, element);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(Remove)(void **object) 
{
  List *next = List_Next(this);

  if (!List_Next(next)) {
    List_alloc(this, object);

    memset(BASE(0)->second.object, 0, sizeof(void*));

    DELETE (next);
  } else {
    List_Remove(next, object);
  }
}

////////////////////////////////////////////////////////////////////////////////
List *_(Set)(int index, const void *element)
{
  if (!index) {
    if (this->object) {
      void *object = List_Head(this);

      DELETE (object);
    }

    Pair_SetF(BASE(0), &element);

    return this;
  } else {
    return List_Set(List_Next(this), index - 1, element);
  }
}

////////////////////////////////////////////////////////////////////////////////
List *_(Insert)(int index, const void *element)
{
  List *next = List_Next(this);

  if (!index) {
    List *insert = NEW (List)();
    void *head   = List_Head(this);

    Pair_SetF(&insert->base, &head);
    Pair_SetS(&insert->base, &next);

    insert->object = this->object;
    this->object   = 0; // default

    Pair_SetF(BASE(0), &element);
    Pair_SetS(BASE(0), &insert);

    return this;
  } else {
    return List_Insert(next, index - 1, element);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveAt)(int index, void **object)
{
    List *next = List_Next(this);

  if (!index) {
    void *head    = List_Head(next);
    List *further = List_Next(next);

    List_alloc(this, object);

    Pair_SetF(BASE(0), &head);
    Pair_SetS(BASE(0), &further);

    Pair_Destruct(&next->base);
    tfree(next);
  } else {
    List_RemoveAt(next, index - 1, object);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(Merge)(List *other)
{
  List *next = List_Next(this);

  if (!next) {
    void *head = List_Head(other);

    next = List_Next(other);

    Pair_SetF(BASE(0), &head);
    Pair_SetS(BASE(0), &next);

    Pair_Destruct(&other->base);
    tfree(other);
  } else {
    List_Merge(next, other);
  }
}


#undef TYPENAME