#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
Map *_(cons)(size_t size_first, size_t size_second) {
  if (_this) {
    Array_cons((Array*)_this, sizeof(PAIR_PTR(size_first, size_second)));
    _this->size_first  = size_first;
    _this->size_second = size_second;
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)() {
  Array_free((Array*)_this);
}

////////////////////////////////////////////////////////////////////////////////
void *_(atkey)(void *key) {
  for (int i = 0; i < _this->base.size; i++) {
    void *index = (char*)_this->base.base + i * _this->base.element_size;
    
    if (!memcmp(key, index, _this->size_first)) return index;
  }

  return NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *_(vatkey)(void *key) {
  return (void*)((char*)Map_atkey(_this, key) + _this->size_first);
}

////////////////////////////////////////////////////////////////////////////////
void *_(setkey)(void *key) {
  void *place = Map_vatkey(_this, key);

  if (!place) {
    PAIR_PTR(_this->size_first, _this->size_second) tmp;

    memcpy(&tmp.first,  key, _this->size_first);
    memset(&tmp.second, 0,   _this->size_second);
    Array_push((Array*)_this, &tmp);
    place = Map_vatkey(_this, key);
  }

  return place;
}
