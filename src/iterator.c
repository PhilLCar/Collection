#include <iterator.h>

//TODO: remove
#include <objectarray.h>

#define TYPENAME Iterator

Iterator *_(Construct)(void *collection)
{
  if (this) {
    const Type      *type      = gettype(collection);
    VirtualFunction  construct = virtual(type, STRINGIZE(TYPENAME));

    if (construct) {
      construct(collection, this);
    } else {
      THROW (NEW (Exception) ("The object '%s' doesn't implement '%s'", type->name, STRINGIZE(TYPENAME)));
    }
  }

  return this;
}

void _(Destruct)()
{
  if (this->end) {
    this->end(this);
    this->end = NULL;
  }
}

int done(Iterator *e)
{
  int done = e->done(e);

  if (done) DELETE(e);

  return done;
}

void next(Iterator *e)
{
  e->next(e);
}

int count(Iterator *e)
{
  return e->count(e);
}

#undef TYPENAME