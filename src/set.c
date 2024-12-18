#include <set.h>

#define TYPENAME Set

#define MAX(A, B) ({\
  __typeof__(A) a = A;\
  __typeof__(B) b = B;\
  a > b ? a : b;\
})\

#define MIN(A, B) ({\
  __typeof__(A) a = A;\
  __typeof__(B) b = B;\
  a < b ? a : b;\
})\

////////////////////////////////////////////////////////////////////////////////
Set *_(Construct)(const Type *type)
{
  if (ObjectArray_Construct(BASE(0), type)) {
    this->comparer    = comparer(type);
    this->keyComparer = key_comparer(type);
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    ObjectArray_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
int CONST (contains)(const void *data, Comparer comparer, int *index)
{
  int size  = BASE(1)->size;
  int slice = size;
  
  *index = 0;

  while (slice) {
    int cmp = comparer(Array_At(BASE(1), *index), data);

    // Half the set of possibilities (when slice is greater than one: round up);
    slice = (slice + (slice > 1)) >> 1;

     if (cmp > 0) {
      // Do not exit the search field;
      slice = MIN(slice, size - *index - 1);
      // The behaviour of Insert places the new element before the specified index, we always need to move up
      *index += MAX(1, slice);
    } else if (cmp < 0) {
      // Do not exit the search field
      slice = MIN(slice, *index);
      *index -= slice;
    } else {
      return 1;
    }
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Contains)(const void *data)
{
  int index;
  
  if (!Set_contains(this, data, this->comparer, &index)) {
    index = -1;
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (ContainsKey)(const void *data)
{
  int index;
  
  if (!Set_contains(this, data, this->keyComparer, &index)) {
    index = -1;
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Add)(void *data)
{
  return Set_AddValue(this, NULL, data);
}

////////////////////////////////////////////////////////////////////////////////
void *_(AddValue)(const Type *type, void *data)
{
  void *result = NULL;
  int   index;

  if (!Set_contains(this, data, this->comparer, &index)) {
    result = ObjectArray_InsertValue(BASE(0), index, NULL, data);
  } else {
    DELETE (data);
  }

  print("%O\n", this);
  
  return result;
}

////////////////////////////////////////////////////////////////////////////////
int _(Remove)(const void *data)
{
  int index, found;

  if ((found = Set_contains(this, data, this->comparer, &index))) {
    ObjectArray_RemoveAt(BASE(0), index, 0);
  }

  return found;
}

////////////////////////////////////////////////////////////////////////////////
int _(RemoveKey)(const void *data)
{
  int index, found;

  if ((found = Set_contains(this, data, this->keyComparer, &index))) {
    ObjectArray_RemoveAt(BASE(0), index, 0);
  }

  return found;
}

////////////////////////////////////////////////////////////////////////////////
Set *_(Union)(Set *other)
{
  while (other->base.base.size) {
    Set_Add(this, ObjectArray_Pop(&other->base, 1));
  }

  DELETE (other);

  return this;
}

////////////////////////////////////////////////////////////////////////////////
Set *_(Intersect)(Set *other)
{
  for (int i = BASE(1)->size - 1; i >= 0; i--) {
    void *object = Array_At(BASE(1), i);

    if (!Set_Contains(other, object)) {
      ObjectArray_Pop(BASE(0), 0);
    }
  }

  DELETE (other);

  return this;
}

#undef TYPENAME
