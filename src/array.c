#include <array.h>

#define TYPENAME Array

////////////////////////////////////////////////////////////////////////////////
Array *_(cons)(size_t element_size)
{
  if (this) {
    void *base = calloc(element_size, 1);

    if (base) {
      this->base          = base;
      this->size          = 0;
      this->capacity      = 1;
      this->element_size  = element_size;
    } else {
      tfree(this);
      this = NULL;
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (this) {
    free(this->base);
  }
}

////////////////////////////////////////////////////////////////////////////////
Array *_(fill)(...)
{
  void    *arg;
  va_list  args;

  va_start(args, this);
  while ((arg = va_arg(args, void*))) {
    Array_push(this, arg);
  }
  va_end(args);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
int _(recap)(int new_cap)
{
  int success = 0;

  if (this->size > new_cap) {
    this->size = new_cap;
  }

  if (this->capacity != new_cap) {
    void *tmp = reallocarray(this->base, this->element_size, (size_t)new_cap);

    if (tmp != NULL) {
      this->base     = tmp;
      this->capacity = new_cap;
      success = 1;
    }
  }
  
  return success;
}

////////////////////////////////////////////////////////////////////////////////
int _(resize)(int new_size)
{
  int success = new_size <= this->capacity;

  if (!success && (success = Array_recap(this, new_size ? new_size << 1 : 1))) {
    memset((char*)this->base + (this->element_size * this->size), 0, new_size - this->size);
  }

  if (success) {
    this->size = new_size;
  }
  
  return success;
}

////////////////////////////////////////////////////////////////////////////////
void _(push)(void *data)
{
  if (this->size >= this->capacity) {
    void *prevloc = NULL;

    if ((char*)data >= (char*)this->base && 
        (char*)data <  (char*)this->base + (this->element_size * this->size))
    { // array is copying itself, update pointer
      prevloc = this->base;
    }
    if (!Array_recap(this, this->capacity << 1)) return;
    if (prevloc) {
      data = (char*)data + ((long)this->base - (long)prevloc);
    }
  }

  memcpy((char*)this->base + (this->element_size * this->size++),
	       (char*)data,
	       this->element_size);
}

////////////////////////////////////////////////////////////////////////////////
void *_(pop)()
{
  void *index = NULL;

  if (this->size > 0) {
    int size     = this->size--;
    int capacity = this->capacity;

    index = (char*)this->base + (this->size * this->element_size);

    if (size > 1 && size < (capacity >> 2)) {
      Array_recap(this, capacity >> 1);
    }
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
void *_(popptr)()
{
  return *(void**)Array_pop(this);
}

////////////////////////////////////////////////////////////////////////////////
int _(index)(int *index)
{
  if (*index < 0) *index += this->size;

  return *index >= 0 && *index < this->size;
}

////////////////////////////////////////////////////////////////////////////////
void *_(at)(int index)
{
  char *address = NULL;

  if (Array_index(this, &index)) {
    address = (char*)this->base + (index * this->element_size);
  }

  return address;
}

////////////////////////////////////////////////////////////////////////////////
void *_(atptr)(int index)
{
  char *address = NULL;

  if (Array_index(this, &index)) {
    address = ((void**)this->base)[index];
  }

  return address;
}

////////////////////////////////////////////////////////////////////////////////
void *_(last)()
{
  return Array_at(this, -1);
}

////////////////////////////////////////////////////////////////////////////////
void *_(lastptr)()
{
  return Array_atptr(this, -1);
}

////////////////////////////////////////////////////////////////////////////////
int _(remrange)(int start, int range)
{
  int removed = 0;

  if (Array_index(this, &start)) {
    if (start + range > this->size)
    {
      range = this->size - start;
    }

    memcpy((char*)this->base +  (start           * this->element_size),
           (char*)this->base + ((start + range)  * this->element_size),
                 (this->size -  (start + range)) * this->element_size);

    this->size -= (removed = range);
  }

  return removed;
}

////////////////////////////////////////////////////////////////////////////////
void *_(rem)(int index)
{
  void *rem = NULL;

  if (Array_index(this, &index)) {
    void *tmp = malloc(this->element_size);

    if (tmp) {
      memcpy(tmp, (char*)this->base + (index * this->element_size), this->element_size);
      memcpy((char*)this->base + (index       * this->element_size),
             (char*)this->base + ((index + 1) * this->element_size),
             (this->size - index) * this->element_size);
      rem = (char*)this->base + (--this->size * this->element_size);
      memcpy(rem, tmp, this->element_size);
      free(tmp);
    }
  }

  return rem;
}

////////////////////////////////////////////////////////////////////////////////
void *_(remptr)(int index)
{
  return *(void**)Array_rem(this, index);
}

////////////////////////////////////////////////////////////////////////////////
void _(clear)()
{
  this->size = 0;
}

////////////////////////////////////////////////////////////////////////////////
void _(set)(int index, void *value)
{
  if (Array_index(this, &index)) {
    memcpy((char*)this->base + (index * this->element_size), value, this->element_size);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(insert)(int index, void *data)
{
  if (Array_index(this, &index)) {
    if (this->size >= this->capacity) {
      void *prevloc = NULL;

      if ((char*)data >= (char*)this->base && 
          (char*)data <  (char*)this->base + (this->element_size * this->size))
      { // array is copying itself, update pointer
        prevloc = this->base;
      }
      if (!Array_recap(this, this->capacity << 1)) return;
      if (prevloc) {
        data = (char*)data + ((long)this->base - (long)prevloc);
      }
    }
    // Array is copying the moving part
    if ((char*)data >= (char*)this->base + index      * this->element_size &&
        (char*)data <  (char*)this->base + this->size * this->element_size) {
      data = (char*)data + this->element_size;
    }
    memcpy((char*)this->base + (index + 1) * this->element_size, 
           (char*)this->base +  index      * this->element_size,
           (this->size++     -  index    ) * this->element_size);
    memcpy((char*)this->base +  index      * this->element_size, 
          (char*)data,                       this->element_size);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(addrange)(Array *that)
{
  if (this->element_size == that->element_size) {
    int size = this->size;

    Array_resize(this, this->size + that->size);
    memcpy((char*)this->base + (size * this->element_size), that->base, that->size * that->element_size);
  }

  DELETE (that);
}

////////////////////////////////////////////////////////////////////////////////
void *_(in)(void *data)
{
  void *contains = NULL;
  for (int i = 0; i < this->size; i++) {
    void *tmp = (char*)this->base + i * this->element_size;

    if(!memcmp((char*)data, tmp, this->element_size)) {
      contains = tmp;
      break;
    }
  }

  return contains;
}

////////////////////////////////////////////////////////////////////////////////
void *_(inptr)(void *data)
{
  void **ptr = Array_in(this, data);

  if (ptr) return *ptr;
  
  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
int _(indexof)(void *data)
{
  int index = -1;

  for (int i = 0; i < this->size; i++) {
    if(!memcmp((char*)data, (char*)this->base + i * this->element_size, this->element_size)) {
      index = i;
      break;
    }
  }

  return index;
}