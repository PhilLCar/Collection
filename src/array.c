#include <array.h>

#define TYPENAME Array

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(size_t element_size)
{
  if (_this) {
    void *base = calloc(element_size, 1);

    if (base) {
      _this->base          = base;
      _this->size          = 0;
      _this->capacity      = 1;
      _this->element_size  = element_size;
    } else {
      free(_this);
      _this = NULL;
    }
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (_this) {
    free(_this->base);
  }
}

////////////////////////////////////////////////////////////////////////////////
Array *_(fill)(...)
{
  void    *arg;
  va_list  args;

  va_start(args, _this);
  while ((arg = va_arg(args, void*))) {
    Array_push(_this, arg);
  }
  va_end(args);

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
int _(recap)(int new_cap)
{
  int success = 0;

  if (_this->size > new_cap) {
    _this->size = new_cap;
  }

  if (_this->size <= new_cap) {
    void *tmp = reallocarray(_this->base, _this->element_size, (size_t)new_cap);

    if (tmp != NULL) {
      _this->base     = tmp;
      _this->capacity = new_cap;
      success = 1;
    }
  }
  
  return success;
}

////////////////////////////////////////////////////////////////////////////////
int _(resize)(int new_size)
{
  int success = new_size <= _this->capacity;
  
  if (!success)
  {
    success = Array_recap(_this, new_size ? new_size << 1 : 1);
  } 

  if (success) {
    _this->size = new_size;
  }
  
  return success;
}

////////////////////////////////////////////////////////////////////////////////
void _(push)(void *data)
{
  if (_this->size >= _this->capacity) {
    void *prevloc = NULL;

    if ((char*)data >= (char*)_this->base && 
        (char*)data <  (char*)_this->base + (_this->element_size * _this->size))
    { // array is copying itself, update pointer
      prevloc = _this->base;
    }
    if (!Array_recap(_this, _this->capacity << 1)) return;
    if (prevloc) {
      data = (char*)data + ((long)_this->base - (long)prevloc);
    }
  }

  memcpy((char*)_this->base + (_this->element_size * _this->size++),
	       (char*)data,
	       _this->element_size);
}

////////////////////////////////////////////////////////////////////////////////
void *_(pop)()
{
  void *index = NULL;
  if (_this->size > 0) {
    int size     = _this->size--;
    int capacity = _this->capacity;
    index = (char*)_this->base + (_this->size * _this->element_size);
    if (size > 1 && size < capacity >> 2) {
      Array_recap(_this, capacity >> 1);
    }
  }
  return index;
}

////////////////////////////////////////////////////////////////////////////////
void *_(popp)()
{
  return *(void**)Array_pop(_this);
}

////////////////////////////////////////////////////////////////////////////////
void *_(at)(int index)
{
  char *address = NULL;

  if (index < 0) index += _this->size;
  if (index >= 0 && index < _this->size) {
    address = (char*)_this->base + (index * _this->element_size);
  }

  return address;
}

////////////////////////////////////////////////////////////////////////////////
void *_(atp)(int index)
{
  char *address = NULL;

  if (index < 0) index += _this->size;
  if (index >= 0 && index < _this->size) {
    address = ((void**)_this->base)[index];
  }

  return address;
}

////////////////////////////////////////////////////////////////////////////////
void *_(last)()
{
  return Array_at(_this, -1);
}

////////////////////////////////////////////////////////////////////////////////
void *_(lastp)()
{
  return Array_atp(_this, -1);
}

////////////////////////////////////////////////////////////////////////////////
int _(remrange)(int start, int range)
{
  int removed = 0;

  if (start < 0) start += _this->size;
  if (start >= 0) {
    if (start + range > _this->size)
    {
      range = _this->size - start;
    }

    memcpy((char*)_this->base +  (start           * _this->element_size),
           (char*)_this->base + ((start + range)  * _this->element_size),
                 (_this->size -  (start + range)) * _this->element_size);

    _this->size -= (removed = range);
  }

  return removed;
}

////////////////////////////////////////////////////////////////////////////////
void *_(rem)(int index)
{
  void *rem = NULL;

  if (index < 0) index += _this->size;
  if (index >= 0 && index < _this->size) {
    void *tmp = malloc(_this->element_size);

    if (tmp) {
      memcpy(tmp, (char*)_this->base + (index * _this->element_size), _this->element_size);
      memcpy((char*)_this->base      + (index       * _this->element_size),
             (char*)_this->base      + ((index + 1) * _this->element_size),
             (_this->size - index) * _this->element_size);
      rem = (char*)_this->base + (--_this->size * _this->element_size);
      memcpy(rem, tmp, _this->element_size);
      free(tmp);
    }
  }

  return rem;
}

////////////////////////////////////////////////////////////////////////////////
void *_(remp)(int index)
{
  return *(void**)Array_rem(_this, index);
}

////////////////////////////////////////////////////////////////////////////////
void _(clear)()
{
  _this->size = 0;
}

////////////////////////////////////////////////////////////////////////////////
void _(set)(int index, void *value)
{
  if (index < 0) index += _this->size;
  if (index >= 0 && index < _this->size) {
    memcpy((char*)_this->base + (index * _this->element_size), value, _this->element_size);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(insert)(int index, void *data)
{
  if (_this->size >= _this->capacity) {
    void *prevloc = NULL;

    if ((char*)data >= (char*)_this->base && 
        (char*)data <  (char*)_this->base + (_this->element_size * _this->size))
    { // array is copying itself, update pointer
      prevloc = _this->base;
    }
    if (!Array_recap(_this, _this->capacity << 1)) return;
    if (prevloc) {
      data = (char*)data + ((long)_this->base - (long)prevloc);
    }
  }
  // Array is copying the moving part
  if ((char*)data >= (char*)_this->base + index       * _this->element_size &&
      (char*)data <  (char*)_this->base + _this->size * _this->element_size) {
    data = (char*)data + _this->element_size;
  }
  memcpy((char*)_this->base + (index + 1) * _this->element_size, 
         (char*)_this->base +  index      * _this->element_size,
         (_this->size++     -  index    ) * _this->element_size);
  memcpy((char*)_this->base +  index      * _this->element_size, 
         (char*)data,                       _this->element_size);
}

////////////////////////////////////////////////////////////////////////////////
void _(combine)(Array *other)
{
  if (_this->element_size == other->element_size) {
    void *elem;

    while ((elem = Array_pop(other))) Array_push(_this, elem);
  }

  DELETE (other);
}

////////////////////////////////////////////////////////////////////////////////
void *_(in)(void *data)
{
  void *contains = NULL;
  for (int i = 0; i < _this->size; i++) {
    void *tmp = (char*)_this->base + i * _this->element_size;
    if(!memcmp((char*)data, tmp, _this->element_size)) {
      contains = tmp;
      break;
    }
  }
  return contains;
}

////////////////////////////////////////////////////////////////////////////////
void *_(inp)(void *data)
{
  void **ptr = Array_in(_this, data);

  if (ptr) return *ptr;
  
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
int _(indexof)(void *data)
{
  int index = -1;

  for (int i = 0; i < _this->size; i++) {
    if(!memcmp((char*)data, (char*)_this->base + i * _this->element_size, _this->element_size)) {
      index = i;
      break;
    }
  }
  return index;
}