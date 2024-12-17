#include <objectarray.h>

#define TYPENAME ObjectArray

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(Construct)(const Type *type)
{
  if (Array_Construct(BASE(0), type->size)) {
    this->type   = type;
    this->buffer = talloc(type);
  }
  
  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    tfree(this->buffer);
    ObjectArray_Clear(this);
    Array_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(Fill)(...)
{
  va_list  argptr;
  void    *current;

  va_start(argptr, this);

  while ((current = va_arg(argptr, void*))) {
    ObjectArray_Push(this, current);  
  }

  va_end(argptr);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(FillValues)(const Type *type, ...)
{
  va_list  argptr;
  void    *current;

  va_start(argptr, type);

  while ((current = va_arg(argptr, void*))) {
    ObjectArray_PushValue(this, type, current);  
  }

  va_end(argptr);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
int _(Resize)(int newSize)
{
  int success = 1;

  if (BASE(0)->size < newSize) {
    int size = BASE(0)->size;

    if ((success = Array_Resize(BASE(0), newSize)) && isobject(this->type)) {
      for (int i = size; i < newSize; i++) {
        this->type->construct(Array_At(BASE(0), i));
      }
    }
  } else if (BASE(0)->size > newSize) {
    if (isobject(this->type)) {
      for (int i = newSize; i < BASE(0)->size; i++) {
        this->type->destruct(Array_At(BASE(0), i));
      }
    }

    success = Array_Resize(BASE(0), newSize);
  }

  return success;
}

/******************************************************************************/
int _(typecheck)(const Type *type, void *data)
{
  int         result   = 1;
  const Type *dataType = IFNULL(type, gettype(data));
  
  if (!sametype(this->type, dataType)) {
    THROW (NEW (Exception) ("Type mismatch! Expected '%s', got '%s'", this->type->name, dataType->name));
    
    result = 0;
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Insert)(int index, void *data)
{
  return ObjectArray_InsertValue(this, index, NULL, data);
}

////////////////////////////////////////////////////////////////////////////////
void *_(InsertValue)(int index, const Type *type, void *data)
{
  void *element = NULL;

  if (ObjectArray_typecheck(this, type, data)) {
    element = Array_Insert(BASE(0), index, data);

    if (!type) tfree(data);
  }

  return element;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Push)(void *data)
{
  return ObjectArray_PushValue(this, NULL, data);
}

void *_(PushValue)(const Type *type, void *data)
{
  void *element = NULL;

  if (ObjectArray_typecheck(this, type, data)) {
    element = Array_Push(BASE(0), data);

    if (!type) tfree(data);
  }

  return element;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Add)(void *data)
{
  return ObjectArray_AddValue(this, NULL, data);
}

////////////////////////////////////////////////////////////////////////////////
void *_(AddValue)(const Type *type, void *data)
{
  void *element = NULL;

  if (ObjectArray_typecheck(this, type, data)) {
    element = Array_Add(BASE(0), data);

    if (!type) tfree(data);
  }

  return element;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Set)(int index, void *data)
{
  return ObjectArray_SetValue(this, index, NULL, data);
}

////////////////////////////////////////////////////////////////////////////////
void *_(SetValue)(int index, const Type *type, void *data)
{
  void *element = NULL;

  if (ObjectArray_typecheck(this, type, data)) {
    if (isobject(this->type)) {
      this->type->destruct(Array_At(BASE(0), index));
    }

    element = Array_Set(BASE(0), index, data);

    if (!type) tfree(data);
  }

  return element;
}

/******************************************************************************/
void *_(alloc)(void *returned)
{
  void *elem = talloc(this->type);

  memcpy(elem, returned, this->type->size);

  return elem;
}

////////////////////////////////////////////////////////////////////////////////
void *_(RemoveAt)(int index, int get)
{
  void *removed = Array_RemoveAt(BASE(0), index);
  
  if (isobject(this->type)) {
    if (get) {
      removed = ObjectArray_alloc(this, removed);
    } else {
      this->type->destruct(removed);
    }
  }

  return removed;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Pop)(int get)
{
  void *popped = Array_Pop(BASE(0));

  if (isobject(this->type)) {
    if (get) {
      popped = ObjectArray_alloc(this, popped);
    } else {
      this->type->destruct(popped);
    }
  }

  return popped;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Remove)(int get)
{
  return ObjectArray_RemoveAt(this, 0, get);
}

////////////////////////////////////////////////////////////////////////////////
int _(RemoveRange)(int start, int range)
{
  int removed = 0;

  if (Array_Index(BASE(0), &start)) {
    if (start + range > BASE(0)->size) {
      range = BASE(0)->size - start;
    }

    // delete the objects first
    if (isobject(this->type)) {
      for (int i = start; i < start + range; i++) {
        this->type->destruct(Array_At(BASE(0), i));
      }
    }
    
    // Then remove the range
    removed = Array_RemoveRange(BASE(0), start, range);
  }

  return removed;
}

////////////////////////////////////////////////////////////////////////////////
void _(Clear)()
{
  ObjectArray_RemoveRange(this, 0, BASE(0)->size);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (At)(int index)
{
  void *at;

  if ((at = Array_At(BASE(0), index))) {
    memcpy(this->buffer, at, this->type->size);

    at = this->buffer;
  }

  return at;
}

/******************************************************************************/
void *CONST (contains)(const void *reference, Comparer compare)
{
  void *found = NULL;

  for (int i = 0; i < BASE(0)->size; i++) {
    void *against = Array_At(BASE(0), i);

    if (!compare(against, reference)) {
      found = against;
      break;
    }
  }

  return found;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (Contains)(const void *reference)
{
  return ObjectArray_contains(this, reference, comparer(this->type));
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ContainsKey)(const void *reference)
{
  return ObjectArray_contains(this, reference, key_comparer(this->type));
}

////////////////////////////////////////////////////////////////////////////////
int CONST (indexOf)(const void *reference, Comparer compare)
{
  int index = -1;

  for (int i = 0; i < BASE(0)->size; i++) {
    void *against = Array_At(BASE(0), i);

    if (!compare(against, reference)) {
      index = i;
      break;
    }
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (IndexOf)(const void *reference)
{
  return ObjectArray_indexOf(this, reference, comparer(this->type));
}

////////////////////////////////////////////////////////////////////////////////
int CONST (IndexOfKey)(const void *reference)
{
  return ObjectArray_indexOf(this, reference, key_comparer(this->type));
}

/******************************************************************************/
void STATIC(next)(Iterator *iterator)
{
  ObjectArray_At(iterator->collection, ++*((int*)iterator->env));
}

int  Array_done(Iterator*);
void Array_end(Iterator*);
int  Array_count(Iterator*);

////////////////////////////////////////////////////////////////////////////////
void _(Iterator)(Iterator *iterator)
{
  iterator->base       = ObjectArray_At(this, 0);
  iterator->collection = this;
  iterator->env        = malloc(sizeof(int));
  iterator->done       = Array_done;
  iterator->next       = ObjectArray_next;
  iterator->end        = Array_end;
  iterator->count      = Array_count;

  *((int*)iterator->env) = 0;
}

#undef TYPENAME
