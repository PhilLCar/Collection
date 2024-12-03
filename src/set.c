#include <set.h>

#define TYPENAME Set

////////////////////////////////////////////////////////////////////////////////
Set *_(Construct)(const Type *type, Comparer compare)
{
  if (ObjectArray_Construct(BASE(0), type)) {
    this->compare = compare ? compare : default_comparer;
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
void *_(Add)(void *data)
{
  for (int i = 0; i < BASE(1)->size; i++) {
    int cmp = this->compare(Array_At(BASE(1), i), data);

    if (cmp < 0) {
      return ObjectArray_Insert(BASE(0), i, data);
    } else if (!cmp) {
      return NULL;
    }
  }
  
  return ObjectArray_Push(BASE(0), data);
}

#undef TYPENAME
