#include <pair.h>

#define TYPENAME Pair

/******************************************************************************/
void STATIC (new)(PairMember *member)
{
  if (isobject(&member->type)) {
    member->object = talloc(&member->type);
    member->type.new(member->object);
  } else {
    member->object = malloc(member->type.size);
  }
}

/******************************************************************************/
void STATIC (delete)(PairMember *member)
{
  if (isobject(&member->type)) {
    member->type.delete(member->object);
    tfree(member->object);
  } else {
    free(member->object);
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(cons)(Type first, Type second)
{
  if (this) {
    memcpy(&this->first.type,  &first,  sizeof(Type));
    memcpy(&this->second.type, &second, sizeof(Type));
    
    Pair_new(&this->first);
    Pair_new(&this->second);

    if (!this->first.object || !this->second.object) {
      Pair_free(this);

      this = NULL;
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(free)()
{
  if (this) {
    Pair_delete(&this->first);
    Pair_delete(&this->second);
  }
}

////////////////////////////////////////////////////////////////////////////////
Pair *STATIC (from)(void *first, void *second)
{
  Pair *pair = NEW (Pair) (*gettype(first), *gettype(second));

  Pair_set(&pair->first,  first);
  Pair_set(&pair->second, second);

  return pair;
}

////////////////////////////////////////////////////////////////////////////////
void *STATIC (set)(PairMember *member, void *element)
{
  int object = isobject(&member->type);

  if (!object || sametype(&member->type, gettype(element))) {
    if (object) member->type.delete(member->object);
    memcpy(member->object, element, member->type.size);
    if (object) tfree(element);
  }

  return member->object;
}

////////////////////////////////////////////////////////////////////////////////
void *_(setf)(void *element)
{
  return Pair_set(&this->first, element);
}

////////////////////////////////////////////////////////////////////////////////
void *_(sets)(void *element)
{
  return Pair_set(&this->second, element);
}