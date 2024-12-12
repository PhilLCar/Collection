#include <objectarray.h>

#define TYPENAME ObjectArray

////////////////////////////////////////////////////////////////////////////////
int default_comparer(const void *against, const void *reference) {
	return reference - against;
}

////////////////////////////////////////////////////////////////////////////////
int default_base_comparer(const void **against, const void *reference) {
	return default_comparer(*against, reference);
}

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
  void    *arg;
  va_list  args;

  va_start(args, this);
  while ((arg = va_arg(args, void*))) {
    ObjectArray_Push(this, arg);
  }
  va_end(args);

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

////////////////////////////////////////////////////////////////////////////////
void *_(Insert)(int index, void *data)
{
  void *insert = Array_Insert(BASE(0), index, data);

  if (isobject(this->type)) {
    tfree(data);
  }

  return insert;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Push)(void *data)
{
  void *pushed = NULL;
  int   object = isobject(this->type);

  if (!object || sametype(this->type, gettype(data))) {
    pushed = Array_Push(BASE(0), data);

    if (object) {
      tfree(data);
    }
  }

  return pushed;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Add)(void *data)
{
  return ObjectArray_Insert(this, 0, data);
}

////////////////////////////////////////////////////////////////////////////////
void *_(Set)(int index, void *value)
{
  void *set = NULL;
  int   object = isobject(this->type);

  if (object) {
    this->type->destruct(Array_At(BASE(0), index));
  }

  set = Array_Set(BASE(0), index, value);

  if (object) {
    tfree(value);
  }

  return set;
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
Comparer CONST (comparer)()
{
  return IFNULL(virtual(this->type, "Comparer"), default_comparer);
}

/******************************************************************************/
Comparer CONST (baseComparer)()
{
  return IFNULL(virtual(this->type, "BaseComparer"), default_base_comparer);
}

/******************************************************************************/
void *CONST (in)(const void *reference, Comparer compare)
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
void *CONST (In)(const void *reference)
{
  return ObjectArray_in(this, reference, ObjectArray_comparer(this));
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (BaseIn)(const void *reference)
{
  return ObjectArray_in(this, reference, ObjectArray_baseComparer(this));
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
  return ObjectArray_indexOf(this, reference, ObjectArray_comparer(this));
}

////////////////////////////////////////////////////////////////////////////////
int CONST (BaseIndexOf)(const void *reference)
{
  return ObjectArray_indexOf(this, reference, ObjectArray_baseComparer(this));
}


#undef TYPENAME
