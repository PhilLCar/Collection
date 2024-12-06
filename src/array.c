#include <array.h>

#define TYPENAME Array

////////////////////////////////////////////////////////////////////////////////
Array *_(Construct)(size_t element_size)
{
  if (this) {
    void *base = calloc(element_size, 1);

    if (base) {
      this->base          = base;
      this->size          = 0;
      this->capacity      = 1;
      this->element_size  = element_size;
    } else {
      THROW(NEW (MemoryAllocationException)());
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this && this->base) {
    free(this->base);
    this->base = NULL;
  }
}

////////////////////////////////////////////////////////////////////////////////
Array *_(Fill)(...)
{
  void    *arg;
  va_list  args;

  va_start(args, this);
  while ((arg = va_arg(args, void*))) {
    Array_Push(this, arg);
  }
  va_end(args);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
int _(Recap)(int newCap)
{
  int success = 0;

  if (this->size > newCap) {
    this->size = newCap;
  }

  if (this->capacity != newCap) {
    void *tmp = reallocarray(this->base, this->element_size, (size_t)newCap);

    if (tmp != NULL) {
      this->base     = tmp;
      this->capacity = newCap;
      success = 1;
    }
  }
  
  return success;
}

////////////////////////////////////////////////////////////////////////////////
int _(Resize)(int newSize)
{
  int success = newSize <= this->capacity;

  if (!success && (success = Array_Recap(this, newSize ? newSize << 1 : 1))) {
    memset((char*)this->base + (this->element_size * this->size), 0, newSize - this->size);
  }

  if (success) {
    this->size = newSize;
  }
  
  return success;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Insert)(int index, const void *data)
{
  void *inserted = NULL;

  if (Array_Index(this, &index) || index == this->size) {
    if (this->size >= this->capacity) {
      void *prevloc = NULL;

      if ((char*)data >= (char*)this->base && 
          (char*)data <  (char*)this->base + (this->element_size * this->size))
      { // array is copying itself, update pointer
        prevloc = this->base;
      }
      if (!Array_Recap(this, this->capacity << 1)) return NULL;
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
           (char*)data,                      this->element_size);

    inserted = (char*)this->base + (index * this->element_size);
  }

  return inserted;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Push)(const void *data)
{
  if (this->size >= this->capacity) {
    void *prevloc = NULL;

    if ((char*)data >= (char*)this->base && 
        (char*)data <  (char*)this->base + (this->element_size * this->size))
    { // array is copying itself, update pointer
      prevloc = this->base;
    }
    if (!Array_Recap(this, this->capacity << 1)) return NULL;
    if (prevloc) {
      data = (char*)data + ((long)this->base - (long)prevloc);
    }
  }

  memcpy((char*)this->base + (this->element_size * this->size),
	       (char*)data,
	       this->element_size);

  return (char*)this->base + (this->element_size * this->size++);
}

////////////////////////////////////////////////////////////////////////////////
void *_(Add)(const void *data)
{
  return Array_Insert(this, 0, data);
}

////////////////////////////////////////////////////////////////////////////////
void _(Merge)(Array *that)
{
  if (this->element_size == that->element_size) {
    int size = this->size;

    Array_Resize(this, this->size + that->size);
    memcpy((char*)this->base + (size * this->element_size), that->base, that->size * that->element_size);
  }

  DELETE (that);
}

////////////////////////////////////////////////////////////////////////////////
int _(Swap)(int index1, int index2)
{
  int swapped = 0;

  if (Array_Index(this, &index1) && Array_Index(this, &index2) && index1 != index2) {
    char *buffer = malloc(this->element_size);

    void *ptr1 = (char*)this->base + (index1 * this->element_size);
    void *ptr2 = (char*)this->base + (index2 * this->element_size);

    memcpy(buffer, ptr1,   this->element_size);
    memcpy(ptr1,   ptr2,   this->element_size);
    memcpy(ptr2,   buffer, this->element_size);

    swapped = 1;
  }

  return swapped;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Set)(int index, const void *value)
{
  if (Array_Index(this, &index)) {
    memcpy((char*)this->base + (index * this->element_size), value, this->element_size);
  }

  return (char*)this->base + (index * this->element_size);
}

////////////////////////////////////////////////////////////////////////////////
void *_(RemoveAt)(int index)
{
  void *rem = NULL;

  if (Array_Index(this, &index)) {
    void *tmp = malloc(this->element_size);

    if (tmp) {
      memcpy(tmp, (char*)this->base + (index * this->element_size), this->element_size);
      memcpy((char*)this->base + (index       * this->element_size),
             (char*)this->base + ((index + 1) * this->element_size),
             (this->size - index) * this->element_size);
      rem = (char*)this->base + (--this->size * this->element_size);
      memcpy(rem, tmp, this->element_size);
      free(tmp);

      if (this->size > 0 && this->size < (this->capacity >> 2)) {
        Array_Recap(this, this->capacity >> 1);
      }
    }
  }

  return rem;
}

////////////////////////////////////////////////////////////////////////////////
void *_(RemoveAtDeref)(int index)
{
  void **ptr = Array_RemoveAt(this, 0);

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Pop)()
{
  void *index = NULL;

  if (this->size > 0) {
    int size     = this->size--;
    int capacity = this->capacity;

    index = (char*)this->base + (this->size * this->element_size);

    if (size > 1 && size < (capacity >> 2)) {
      Array_Recap(this, capacity >> 1);
    }
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
void *_(PopPtr)()
{
  return *(void**)Array_Pop(this);
}

////////////////////////////////////////////////////////////////////////////////
void *_(Remove)()
{
  return Array_RemoveAt(this, 0);
}

////////////////////////////////////////////////////////////////////////////////
void *_(RemoveDeref)()
{
  void **ptr = Array_RemoveAt(this, 0);

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
int _(RemoveRange)(int start, int range)
{
  int removed = 0;

  if (Array_Index(this, &start)) {
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
void _(Clear)()
{
  this->size = 0;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Index)(int *index)
{
  if (*index < 0) *index += this->size;

  return *index >= 0 && *index < this->size;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (At)(int index)
{
  char *address = NULL;

  if (Array_Index(this, &index)) {
    address = (char*)this->base + (index * this->element_size);
  }

  return address;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (AtDeref)(int index)
{
  void **ptr = Array_At(this, index);

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (Last)()
{
  return Array_At(this, -1);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (LastDeref)()
{
  return Array_AtDeref(this, -1);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (In)(const void *data)
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
void *CONST (InDeref)(const void *data)
{
  void **ptr = Array_In(this, data);

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (IndexOf)(const void *data)
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

#undef TYPENAME
