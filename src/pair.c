#include <pair.h>

#define TYPENAME Pair

////////////////////////////////////////////////////////////////////////////////
Pair *_(cons)(size_t first_size, size_t second_size)
{
  if (_this) {
    _this->first_size  = first_size;
    _this->first       = malloc(first_size + sizeof(const char*));
    _this->second_size = second_size;
    _this->second      = malloc(second_size + sizeof(const char*));

    if (!_this->first || !_this->second) {
      if (_this->first)  free(_this->first);
      if (_this->second) free(_this->second);

      free(_this);
      _this = NULL;
    } else {
      memset(_this->first,  0, first_size  + sizeof(const char*));
      memset(_this->second, 0, second_size + sizeof(const char*));
    }
  }

  return _this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (_this) {
    Pair_frem(_this);
    Pair_srem(_this);

    free(_this->first);
    free(_this->second);
  }
}

////////////////////////////////////////////////////////////////////////////////
const char* _(fobject)()
{
  return *(const char**)((char*)_this->first + _this->first_size);
}

////////////////////////////////////////////////////////////////////////////////
const char* _(sobject)()
{
  return *(const char**)((char*)_this->second + _this->second_size);
}

////////////////////////////////////////////////////////////////////////////////
void _(frem)()
{
  VirtualFunction free_first  = _virtual("free", Pair_fobject(_this));

  if (free_first != NULL) {
    free_first(_this->first);
  }

  memset(_this->first, 0, _this->first_size);
}

////////////////////////////////////////////////////////////////////////////////
void _(srem)()
{
  VirtualFunction free_second = _virtual("free", Pair_sobject(_this));

  if (free_second != NULL) {
    free_second(_this->second);
  }

  memset(_this->second, 0, _this->second_size);
}

////////////////////////////////////////////////////////////////////////////////
void *_(fset)(void *element)
{
  Pair_frem(_this);
  memcpy(_this->first, element, _this->first_size + sizeof(char*));
  free(element);

  return _this->first;
}

////////////////////////////////////////////////////////////////////////////////
void *_(sset)(void *element)
{
  Pair_srem(_this);
  memcpy(_this->second, element, _this->second_size + sizeof(char*));
  free(element);
  
  return _this->second;
}