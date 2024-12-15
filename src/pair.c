#include <pair.h>

#define TYPENAME Pair

////////////////////////////////////////////////////////////////////////////////
Pair *_(Construct)()
{
  if (this) {    
    this->first  = NULL;
    this->second = NULL;
  } else {
    THROW(NEW (MemoryAllocationException)());
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    DELETE (this->first);
    DELETE (this->second);
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *STATIC (From)(void *first, void *second)
{
  Pair *pair = NEW (Pair) ();

  Pair_SetF(pair, first);
  Pair_SetS(pair, second);

  return pair;
}

/******************************************************************************/
void *STATIC (set)(void **member, void *element)
{
  DELETE (*member);

  return (*member = element);
}

/******************************************************************************/
void *STATIC (setValue)(void **member, const Type *type, void *element)
{
  void *result = NULL;

  if (isobject(type)) {
    result = Pair_set(member, element);
  } else {
    result = talloc(type);
    memcpy(result, element, type->size);
    result = Pair_set(member, result);
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////////
void *_(SetF)(void *element)
{
  return Pair_set(&this->first, element);
}

////////////////////////////////////////////////////////////////////////////////
void *_(SetS)(void *element)
{
  return Pair_set(&this->second, element);
}

////////////////////////////////////////////////////////////////////////////////
void *_(SetValueF)(const Type *type, void *element)
{
  return Pair_setValue(&this->first, type, element);
}

////////////////////////////////////////////////////////////////////////////////
void *_(SetValueS)(const Type *type, void *element)
{
  return Pair_setValue(&this->second, type, element);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (FDeref)()
{
  void **ptr = this->first;

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (SDeref)()
{
  void **ptr = this->second;
  
  return ptr ? *ptr : NULL;
}

#undef TYPENAME
