#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
Map *_(cons)(Type key, Type value, Comparer comparer) {
  if (this && ObjectArray_cons(BASE(0), OBJECT_TYPE(Pair))) {
    memcpy(&this->key,   &key,   sizeof(Type));
    memcpy(&this->value, &value, sizeof(Type));

    this->comparer = comparer;
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  if (this) {
    ObjectArray_free(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(atkey)(const void *key) {
  Pair  *pair  = NULL;

  // Base 0: as ObjectArray
  // Base 1: as Array
  // Base 2: as void*

  for (int i = 0; i < BASE(1)->size; i++) {
    Pair *current = Array_at(BASE(1), i);

    if (this->comparer(current->first.object, key))
    {
      pair = current;
      break;
    }
  }

  return pair;
}

////////////////////////////////////////////////////////////////////////////////
void *_(vatkey)(const void *key) {
  return Map_atkey(this, key)->second.object;
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(setkey)(void *key, void *value) {
  Pair *current = Map_atkey(this, key);

  if (current) {
    Pair_sets(current, value);
  } else {
    current = ObjectArray_push(BASE(0), Pair_from(key, value));
  }

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void _(remkey)(const void *key) {
  Pair *pair  = Map_atkey(this, key);

  if (pair)
  {
    int index = ((char*)pair - (char*)BASE(2)) / BASE(1)->element_size;

    ObjectArray_rem(BASE(0), index, 0);
  }
}