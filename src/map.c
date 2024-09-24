#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(size_t key_size, size_t value_size, Comparer comparer) {
  if (_this) {
    ObjectArray_cons(BASE(0), sizeof(Pair));

    _this->key_size   = key_size;
    _this->value_size = value_size;
    _this->comparer   = comparer;
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  if (_this) {
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
    Pair *current = (Pair*)((char*)BASE(2) + i * BASE(1)->element_size);

    if (_this->comparer(current->first, key))
    {
      pair = current;
      break;
    }
  }

  return pair;
}

////////////////////////////////////////////////////////////////////////////////
void *_(vatkey)(const void *key) {
  return Map_atkey(_this, key)->second;
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(setkey)(void *key, void *value) {
  Pair *current = Map_atkey(_this, key);

  if (current) {
    Pair_sset(current, value);
  } else {
    Pair *pair = NEW (Pair) (_this->key_size, _this->value_size);

    Pair_fset(pair, key);
    Pair_sset(pair, value);

    current = ObjectArray_push(BASE(0), pair);
  }

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void _(remkey)(const void *key) {
  Pair *pair  = Map_atkey(_this, key);
  int   index = Array_indexof(BASE(1), pair);

  if (index >= 0)
  {
    ObjectArray_rem(BASE(0), index);
  }
}