#include <list.h>

#define TYPENAME List

List *_(Construct)()
{
  // Empty list
  return (List*)Pair_Construct(BASE(0), NATIVE_TYPE(void*), NATIVE_TYPE(void*));
}

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

void *CONST (Head)()
{
  return BASE(0)->first.object;
}

List *CONST (Next)()
{
  return Pair_DerefS(BASE(0));
}

void *CONST (At)(int index)
{
  if (index) {
    List *next = List_Next(this);

    return List_At(next , index - 1);
  }

  return List_Head(this);
}

int CONST (Size)()
{
  List *next = List_Next(this);
   
  if (next) return 1 + List_Size(next);

  return 0;
}

void *CONST (In)(const void *element, Comparer compare)
{
  void *head = List_Head(this);
  void *next = List_Next(this);

  if (!next)                   return NULL;
  if (!compare(head, element)) return head;

  return List_In(next, element, compare);
}

List *CONST (Push)(const void *element)
{
  List *list = NEW (List)();

  Pair_SetF((Pair*)list, (void*)element);
  Pair_SetS((Pair*)list, &this);

  return list;
}

////////////////////////////////////////////////////////////////////////////////
List *_(Fill)(...)
{
  void    *arg;
  va_list  args;

  va_start(args, this);
  while ((arg = va_arg(args, void*))) {
    List_Add(this, arg);
  }
  va_end(args);

  return this;
}

void _(alloc)(void** object)
{
  if (object) {
    *object = List_Head(this);

    BASE(0)->first.object = malloc(sizeof(void*));
    memset(BASE(0)->first.object, 0, sizeof(void*));
  } else if (this->object) {
    void *obj = List_Head(this);

    DELETE (obj);
  }
}

List *_(Pop)(void **object)
{
  List *next = List_Next(this);

  List_alloc(this, object);

  Pair_Destruct(BASE(0));
  tfree(this);

  return next;
}

void _(Add)(const void *element)
{
  List *next = List_Next(this);

  if (!next) {
    next = NEW (List)();

    Pair_SetF(BASE(0), (void*)element);
    Pair_SetS(BASE(0), &next);
  } else {
    List_Add(next, element);
  }
}

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

void _(Set)(int index, const void *element)
{
  if (!index) {
    if (this->object) {
      void *object = List_Head(this);

      DELETE (object);

      Pair_SetF(BASE(0), (void*)element);
    }
  } else {
    return List_Set(List_Next(this), index - 1, element);
  }
}

void _(Insert)(int index, const void *element)
{
  List *next = List_Next(this);

  if (!index) {
    List *insert = NEW (List)();

    Pair_SetF(&insert->base, List_Head(this));
    Pair_SetS(&insert->base, &next);

    Pair_SetF(BASE(0), (void*)element);
    Pair_SetS(BASE(0), &insert);
  } else {
    List_Insert(next, index - 1, element);
  }
}

void _(RemoveAt)(int index, void **object)
{
    List *next = List_Next(this);

  if (!index) {
    List *further = List_Next(next);

    List_alloc(this, object);

    Pair_SetF(BASE(0), List_Head(next));
    Pair_SetS(BASE(0), &further);

    Pair_Destruct(&next->base);
    tfree(next);
  } else {
    List_RemoveAt(next, index - 1, object);
  }
}

void _(Merge)(List *other)
{
  List *next = List_Next(this);

  if (!next) {
    next = List_Next(other);

    Pair_SetF(BASE(0), List_Head(other));
    Pair_SetS(BASE(0), &next);

    Pair_Destruct(&other->base);
    tfree(other);
  } else {
    List_Merge(next, other);
  }
}


#undef TYPENAME