#include <set.h>

#define TYPENAME Set

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
int CONST (contains)(const void *data, Comparer comparer)
{
  int slice = BASE(1)->size >> 1;
  int index = slice;

  while (index < BASE(1)->size) {
    int cmp = comparer(Array_At(BASE(1), index), data);

    if (cmp > 0) {
      index += MIN(1, slice);
    } else if (cmp < 0) {
      index -= slice;
    } else {
      index = -1;
      break;
    }

    if (!slice) break;

    slice >>= 1;
  }

  return index;
}

////////////////////////////////////////////////////////////////////////////////
int CONST (Contains)(const void *data)
{
  return Set_contains(this, data, this->comparer);
}

////////////////////////////////////////////////////////////////////////////////
int CONST (ContainsKey)(const void *data)
{
  return Set_contains(this, data, this->keyComparer);
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
  int   index  = Set_Contains(this, data);

  if (index >= 0) {
    result = ObjectArray_InsertValue(BASE(0), index, NULL, data);
  } else {
    DELETE (data);
  }
  
  return result;
}

////////////////////////////////////////////////////////////////////////////////
void *_(Remove)(const void *data, int get)
{
  return ObjectArray_RemoveAt(BASE(0), Set_Contains(this, data), get);
}

////////////////////////////////////////////////////////////////////////////////
void *_(RemoveKey)(const void *data, int get)
{
  return ObjectArray_RemoveAt(BASE(0), Set_ContainsKey(this, data), get);
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
