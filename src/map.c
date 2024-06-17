#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
TYPENAME *_(cons)(size_t key_size, size_t value_size, Comparer comparer) {
  if (_this) {
    ObjectArray_cons(&_this->base, sizeof(Pair) + sizeof(const char *));

    _this->key_size   = key_size;
    _this->value_size = value_size;
    _this->comparer   = comparer;
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  if (_this) {
    ObjectArray_free(&_this->base);
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(atkey)(void *key) {
  Array *array = &_this->base.base;
  Pair  *pairs = (Pair*)array->base;
  Pair  *pair  = NULL;

  for (int i = 0; i < array->size; i++) {
    Pair *current = &pairs[i];

    if (_this->comparer(key, current->first))
    {
      pair = current;
      break;
    }
  }

  return pair;
}

////////////////////////////////////////////////////////////////////////////////
void *_(vatkey)(void *key) {
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

    ObjectArray_push(&_this->base, pair);
  }

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void _(remkey)(void *key) {
  Pair *pair  = Map_atkey(_this, key);
  int   index = Array_indexof(&_this->base.base, pair);

  if (index >= 0)
  {
    ObjectArray_rem(&_this->base, index);
  }
}