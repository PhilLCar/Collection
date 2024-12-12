#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
Map *_(Construct)(const Type *key, const Type *value) {
  if (ObjectArray_Construct(BASE(0), TYPEOF (Pair))) {
    this->key          = key;
    this->value        = value;
    this->comparer     = IFNULL(virtual(key, "Comparer"),     default_comparer);
    this->baseComparer = IFNULL(virtual(key, "BaseComparer"), default_base_comparer);
  }
  
  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)() {
  if (this) {
    ObjectArray_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(Set)(void *key, void *value) {
  Pair *current = Map_At(this, key);

  if (!current) {
    current = ObjectArray_Push(BASE(0), NEW (Pair) (this->key, this->value));
  }

  Pair_SetF(current, key);
  Pair_SetS(current, value);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void _(Remove)(const void *key) {
  Pair *pair  = Map_At(this, key);

  if (pair)
  {
    int index = ((char*)pair - (char*)BASE(2)) / BASE(1)->element_size;

    ObjectArray_RemoveAt(BASE(0), index, 0);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(BaseRemove)(const void *key) {
  Pair *pair  = Map_BaseAt(this, key);

  if (pair)
  {
    int index = ((char*)pair - (char*)BASE(2)) / BASE(1)->element_size;

    ObjectArray_RemoveAt(BASE(0), index, 0);
  }
}

/******************************************************************************/
Pair *CONST (at)(Comparer comparer, void *(*at)(const Array*, int), const void *key) {
  Pair  *pair  = NULL;

  // Base 0: as ObjectArray
  // Base 1: as Array
  // Base 2: as void*

  for (int i = 0; i < BASE(1)->size; i++) {
    Pair *current = at(BASE(1), i);

    if (!comparer(current->first.object, key))
    {
      pair = current;
      break;
    }
  }

  return pair;
}

////////////////////////////////////////////////////////////////////////////////
Pair *CONST (At)(const void *key) {
  return Map_at(this, this->comparer, Array_At, key);
}

////////////////////////////////////////////////////////////////////////////////
Pair *CONST (BaseAt)(const void *key) {
  return Map_at(this, this->baseComparer, Array_AtDeref, key);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAt)(const void *key) {
  Pair *p = Map_At(this, key);
  
  return p ? p->second.object : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (BaseValueAt)(const void *key) {
  Pair *p = Map_BaseAt(this, key);
  
  return p ? p->second.object : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtDeref)(const void *key) {
  Pair *p = Map_At(this, key);

  return p ? Pair_SDeref(p) : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (BaseValueAtDeref)(const void *key) {
  Pair *p = Map_BaseAt(this, key);

  return p ? Pair_SDeref(p) : NULL;
}

#undef TYPENAME
