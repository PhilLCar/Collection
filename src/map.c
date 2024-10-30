#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
Map *_(Construct)(Type key, Type value, Comparer compare) {
  ObjectArray_Construct(BASE(0), OBJECT_TYPE(Pair));

  if (this) {
    memcpy(&this->key,   &key,   sizeof(Type));
    memcpy(&this->value, &value, sizeof(Type));

    // By default simply compare pointers
    this->compare = compare ? compare : default_comparer;
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
Pair *CONST (At)(const void *key) {
  Pair  *pair  = NULL;

  // Base 0: as ObjectArray
  // Base 1: as Array
  // Base 2: as void*

  for (int i = 0; i < BASE(1)->size; i++) {
    Pair *current = Array_At(BASE(1), i);

    if (!this->compare(current->first.object, key))
    {
      pair = current;
      break;
    }
  }

  return pair;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAt)(const void *key) {
  return Map_At(this, key)->second.object;
}


////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtDeref)(const void *key) {
  return Pair_DerefS(Map_At(this, key));
}

