#include <pair.h>

#define TYPENAME Pair

/******************************************************************************/
void STATIC (construct)(PairMember *member)
{
  if (isobject(&member->type)) {
    member->object = talloc(&member->type);
    member->type.construct(member->object);
  } else {
    member->object = malloc(member->type.size);
    memset(member->object, 0, member->type.size);
  }
}

/******************************************************************************/
void STATIC (destruct)(PairMember *member)
{
  if (member->object) {
    if (isobject(&member->type)) {
      member->type.destruct(member->object);
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
  int object = isobject(&member->type);

  if (!object || sametype(&member->type, gettype(element))) {
    if (object) member->type.destruct(member->object);
    memcpy(member->object, element, member->type.size);
    if (object) tfree(element);
  }

  return member->object;
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(Construct)(Type first, Type second)
{
  if (this) {
    memcpy(&this->first.type,  &first,  sizeof(Type));
    memcpy(&this->second.type, &second, sizeof(Type));
    
    Pair_construct(&this->first);
    Pair_construct(&this->second);

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
  Pair *pair = NEW (Pair) (*gettype(first), *gettype(second));

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
void *CONST (DerefF)()
{
  return *(void**)this->first.object;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (DerefS)()
{
  return *(void**)this->second.object;
}

#undef TYPENAME
