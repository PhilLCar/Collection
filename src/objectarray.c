#include <objectarray.h>

#define TYPENAME ObjectArray

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(cons)(Type type)
{
  if (this && Array_cons(BASE(0), type.size)) {
    memcpy(&this->content, &type, sizeof(Type));
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (this) {
    ObjectArray_clear(this);
    Array_free(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(fill)(...)
{
  void    *arg;
  va_list  args;

  va_start(args, this);
  while ((arg = va_arg(args, void*))) {
    ObjectArray_push(this, arg);
  }
  va_end(args);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
int _(resize)(int new_size)
{
  int success = 1;

  if (BASE(0)->size < new_size) {
    int size = BASE(0)->size;

    if ((success = Array_resize(BASE(0), new_size)) && isobject(&this->content)) {
      for (int i = size; i < new_size; i++) {
        this->content.new(Array_at(BASE(0), i));
      }
    }
  } else if (BASE(0)->size > new_size) {
    if (isobject(&this->content)) {
      for (int i = new_size; i < BASE(0)->size; i++) {
        this->content.delete(Array_at(BASE(0), i));
      }
    }

    success = Array_resize(BASE(0), new_size);
  }

  return success;
}

////////////////////////////////////////////////////////////////////////////////
void *_(push)(void *data)
{
  void *pushed = NULL;

  if (sametype(&this->content, gettype(data))) {
    Array_push(BASE(0), data);

    if (isobject(&this->content)) {
      tfree(data);
    }

    pushed = Array_last(BASE(0));
  }

  return pushed;
}

/******************************************************************************/
void *_(alloc)(void *returned)
{
  void *elem = talloc(&this->content);

  memcpy(elem, returned, this->content.size);

  return elem;
}

////////////////////////////////////////////////////////////////////////////////
void *_(pop)(int get)
{
  void *popped = Array_pop(BASE(0));

  if (isobject(&this->content)) {
    if (get) {
      popped = ObjectArray_alloc(this, popped);
    } else {
      this->content.delete(popped);
    }
  }

  return popped;
}

////////////////////////////////////////////////////////////////////////////////
int _(remrange)(int start, int range)
{
  int removed = 0;

  if (Array_index(BASE(0), &start)) {
    if (start + range > BASE(0)->size) {
      range = BASE(0)->size - start;
    }

    // delete the objects first
    if (isobject(&this->content)) {
      for (int i = start; i < start + range; i++) {
        this->content.delete(Array_at(BASE(0), i));
      }
    }
    
    // Then remove the range
    removed = Array_remrange(BASE(0), start, range);
  }

  return removed;
}


////////////////////////////////////////////////////////////////////////////////
void *_(rem)(int index, int get)
{
  void *removed = Array_rem(BASE(0), index);
  
  if (isobject(&this->content)) {
    if (get) {
      removed = ObjectArray_alloc(this, removed);
    } else {
      this->content.delete(removed);
    }
  }

  return removed;
}

////////////////////////////////////////////////////////////////////////////////
void _(clear)()
{
  ObjectArray_remrange(this, 0, BASE(0)->size);
}

////////////////////////////////////////////////////////////////////////////////
void _(set)(int index, void *value)
{
  int object = isobject(&this->content);

  if (object) {
    this->content.delete(Array_at(BASE(0), index));
  }

  Array_set(BASE(0), index, value);

  if (object) {
    tfree(value);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(insert)(int index, void *data)
{
  Array_insert(BASE(0), index, data);

  if (isobject(&this->content)) {
    tfree(data);
  }
}

////////////////////////////////////////////////////////////////////////////////
void *_(in)(void *reference, Comparer comparer)
{
  void *found = NULL;

  for (int i = 0; i < BASE(0)->size; i++) {
    void *against = Array_at(BASE(0), i);

    if (comparer(against, reference)) {
      found = against;
      break;
    }
  }

  return found;
}

////////////////////////////////////////////////////////////////////////////////
int _(indexof)(void *reference, Comparer comparer)
{
  int index = 0;

  for (int i = 0; i < BASE(0)->size; i++) {
    void *against = Array_at(BASE(0), i);

    if (comparer(against, reference)) {
      index = i;
      break;
    }
  }

  return index;
}