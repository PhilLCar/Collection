#include <map.h>

#define TYPENAME Map

////////////////////////////////////////////////////////////////////////////////
Map *_(Construct)(const Type *key) {
  if (List_Construct(BASE(0))) {
    this->comparer    = comparer(key);
    this->keyComparer = key_comparer(key);
  }
  
  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)() {
  List_Destruct(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(Set)(void *key, void *value) {
  return Map_SetValue(this, key, NULL, value);
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(SetKey)(const void *key, void *value) {
  return Map_SetKeyValue(this, key, NULL, value);
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(SetValue)(void *key, const Type *type, void *value) {
  Pair *current = Map_At(this, key);
  
  if (!current) List_Add(BASE(0), (current = NEW (Pair) ()));

  Pair_SetF(current, key);
  Pair_SetValueS(current, type, value);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
Pair *_(SetKeyValue)(const void *key, const Type *type, void *value) {
  Pair *current = Map_AtKey(this, key);

  if (!current) {
    THROW (NEW (Exception) ("Cannot set key if record doesn't already exist! (try set instead)"));
  }

  Pair_SetValueS(current, type, value);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void STATIC (remove)(List *base, Comparer comparer, const void *key) {
  Pair *head = base->base.first;
  List *next = base->base.second;

  if (head && !comparer(head->first, key)) {
    Pair_SetF(&base->base, next->base.first);

    base->base.second = next->base.second;

    tfree(next);
  } else {
    Map_remove(next, comparer, key);
  }
}


////////////////////////////////////////////////////////////////////////////////
void _(Remove)(const void *key) {
  return Map_remove(BASE(0), this->comparer, key);
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveKey)(const void *key) {
  return Map_remove(BASE(0), this->keyComparer, key);
}

/******************************************************************************/
Pair *STATIC (at)(const List *base, Comparer comparer, const void *key) {
  Pair *head = base->base.first;
  void *next = base->base.second;

  if (!next)                            return NULL;
  else if (!comparer(head->first, key)) return head;

  return Map_at(next, comparer, key);
}

////////////////////////////////////////////////////////////////////////////////
Pair *CONST (At)(const void *key)
{
  return Map_at(BASE(0), this->comparer, key);
}

////////////////////////////////////////////////////////////////////////////////
Pair *CONST (AtKey)(const void *key)
{
  return Map_at(BASE(0), this->keyComparer, key);
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAt)(const void *key) {
  Pair *p = (Pair*)Map_At(this, key);
  
  return p ? p->second : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtKey)(const void *key) {
  Pair *p = Map_AtKey(this, key);
  
  return p ? p->second : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtDeref)(const void *key) {
  Pair *p = Map_At(this, key);

  return p ? Pair_SDeref(p) : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtKeyDeref)(const void *key) {
  Pair *p = Map_AtKey(this, key);

  return p ? Pair_SDeref(p) : NULL;
}

#undef TYPENAME
