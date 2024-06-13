#include <object_array.h>

#define TYPENAME ObjectArray

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(size_t element_size)
{
  if (_this) {
    if (!Array_cons(&_this->base, element_size + sizeof(const char *))) _this = NULL;
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (_this) {
    ObjectArray_clear(_this);
    Array_free(&_this->base);
  }
}

////////////////////////////////////////////////////////////////////////////////
const char* _(object)(int index)
{
  return (char*)_this + ((index + 1) * _this->base.element_size) - sizeof(char*);
}

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(fill)(...)
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
void _(push)(void *data)
{
  Array_push(&_this->base, data);
  free(data);
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

  if (start < 0) start += _this->base.size;
  if (start >= 0) {
    if (start + range > _this->base.size)
    {
      range = _this->base.size - start;
    }

    // delete the objects first
    for (int i = start; i < start + range; i++)
    {
      VirtualFunction free_object = _virtual("free", ObjectArray_object(_this, i));
      
      void *index = Array_at(&_this->base, i);

      if (free_object != NULL){
        free_object(index);
      }

      memset(index, 0, _this->base.element_size);
    }
    
    // Then remove the range
    removed = Array_remrange(&_this->base, start, range);
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
  ObjectArray_remrange(_this, 0, _this->base.size);
}

////////////////////////////////////////////////////////////////////////////////
void _(set)(int index, void *value)
{
  Array_set(&_this->base, index, value);
  free(value);
}

////////////////////////////////////////////////////////////////////////////////
void _(insert)(int index, void *data)
{
  Array_insert(&_this->base, index, data);
  free(data);
}