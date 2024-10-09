#include <set.h>

#define TYPENAME Set

////////////////////////////////////////////////////////////////////////////////
Set *_(cons)(Type type, Comparer compare)
{
  if (this && ObjectArray_cons(BASE(0), type)) {
    this->compare = compare ? compare : default_comparer;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (this) {
    ObjectArray_free(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
void *_(add)(void *data)
{
  for (int i = 0; i < BASE(1)->size; i++) {
    int cmp = this->compare(Array_at(BASE(1), i), data);

    if (cmp < 0) {
      return ObjectArray_insert(BASE(0), i, data);
    } else if (!cmp) {
      return NULL;
    }
  }
  
  return ObjectArray_push(BASE(0), data);
}
