#include <set.h>

#define TYPENAME Set

////////////////////////////////////////////////////////////////////////////////
Set *_(Construct)(const Type *type)
{
  if (ObjectArray_Construct(BASE(0), type)) {
    this->comparer = IFNULL(virtual(type, "Comparer"), default_comparer);
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
    int cmp = this->comparer(Array_At(BASE(1), i), data);

    if (cmp < 0) {
      return ObjectArray_Insert(BASE(0), i, data);
    } else if (!cmp) {
      if (isobject(BASE(0)->type)) {
        DELETE (data);
      }

      return NULL;
    }
  }
  
  return ObjectArray_Push(BASE(0), data);
}

#undef TYPENAME
