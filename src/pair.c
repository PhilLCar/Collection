#include <pair.h>

#define TYPENAME Pair

/******************************************************************************/
void STATIC (construct)(PairMember *member, const Type *type)
{
  member->type = type;

  if (isobject(member->type)) {
    member->object = talloc(member->type);
    member->type->construct(member->object);
  } else {
    member->object = malloc(member->type->size);
    memset(member->object, 0, member->type->size);
  }
}

/******************************************************************************/
void STATIC (destruct)(PairMember *member)
{
  if (member->object) {
    if (isobject(member->type)) {
      member->type->destruct(member->object);
      tfree(member->object);
    } else {
      free(member->object);
    }
    member->object = NULL;
  }
}

/******************************************************************************/
void *STATIC (set)(PairMember *member, void *element)
{
  int object = isobject(member->type);

  if (!object || sametype(member->type, gettype(element))) {
    if (object) member->type->destruct(member->object);
    memcpy(member->object, element, member->type->size);
    if (object) tfree(element);
  }

  return member->object;
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(Construct)(const Type *first, const Type *second)
{
  if (this) {    
    Pair_construct(&this->first, first);
    Pair_construct(&this->second, second);

    if (!this->first.object || !this->second.object) {
      THROW(NEW (MemoryAllocationException)());
    }
  } else {
    THROW(NEW (MemoryAllocationException)());
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this) {
    Pair_destruct(&this->first);
    Pair_destruct(&this->second);
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *STATIC (From)(void *first, void *second)
{
  Pair *pair = NEW (Pair) (gettype(first), gettype(second));

  Pair_set(&pair->first,  first);
  Pair_set(&pair->second, second);

  return pair;
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
void *_(F)()
{
  return this->first.object;
}

////////////////////////////////////////////////////////////////////////////////
void *_(S)()
{
  return this->second.object;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (FDeref)()
{
  void **ptr = this->first.object;

  return ptr ? *ptr : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (SDeref)()
{
  void **ptr = this->second.object;
  
  return ptr ? *ptr : NULL;
}

#undef TYPENAME
