#include <object_array.h>

#define TYPENAME ObjectArray

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(cons)(size_t element_size)
{
  if (_this) {
    if (Array_cons(BASE(0), element_size + sizeof(const char*))) {
      memset(BASE(0)->base, 0, BASE(0)->capacity * BASE(0)->element_size);
    } else {
      _this = NULL;
    }
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (_this) {
    ObjectArray_clear(_this);
    Array_free(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
const char* _(object)(int index)
{
  return *(const char**)((char*)BASE(0)->base + ((index + 1) * BASE(0)->element_size) - sizeof(const char*));
}

////////////////////////////////////////////////////////////////////////////////
ObjectArray *_(fill)(...)
{
  void    *arg;
  va_list  args;

  va_start(args, _this);
  while ((arg = va_arg(args, void*))) {
    ObjectArray_push(_this, arg);
  }
  va_end(args);

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void *_(push)(void *data)
{
  Array_push(BASE(0), data);
  free(data);

  return Array_last(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
void _(pop)()
{
  ObjectArray_rem(_this, -1);
}

////////////////////////////////////////////////////////////////////////////////
int _(remrange)(int start, int range)
{
  int removed = 0;

  if (start < 0) start += BASE(0)->size;
  if (start >= 0) {
    if (start + range > BASE(0)->size)
    {
      range = BASE(0)->size - start;
    }

    // delete the objects first
    for (int i = start; i < start + range; i++)
    {
      VirtualFunction free_object = _virtual("free", ObjectArray_object(_this, i));
      
      void *object = Array_at(BASE(0), i);

      if (free_object != NULL) {
        free_object(object);
      }

      memset(object, 0, BASE(0)->element_size);
    }
    
    // Then remove the range
    removed = Array_remrange(BASE(0), start, range);
  }

  return removed;
}


////////////////////////////////////////////////////////////////////////////////
void _(rem)(int index)
{
  ObjectArray_remrange(_this, index, 1);
}

////////////////////////////////////////////////////////////////////////////////
void _(clear)()
{
  ObjectArray_remrange(_this, 0, BASE(0)->size);
}

////////////////////////////////////////////////////////////////////////////////
void _(set)(int index, void *value)
{
  Array_set(BASE(0), index, value);
  free(value);
}

////////////////////////////////////////////////////////////////////////////////
void _(insert)(int index, void *data)
{
  Array_insert(BASE(0), index, data);
  free(data);
}