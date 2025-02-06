#include <mapset.h>

#define TYPENAME MapSet

////////////////////////////////////////////////////////////////////////////////
MapSet *_(Construct)(const Type *key) {
  if (Set_Construct(BASE(0), TYPEOF (KeyVal))) {
    this->env.comparer    = comparer(key);
    this->env.keyComparer = key_comparer(key);
  }
  
  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)() {
  if (this) {
    Set_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *_(Set)(void *key, void *value) {
  return MapSet_SetValue(this, key, NULL, value);
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *_(SetKey)(const void *key, void *value) {
  return MapSet_SetKeyValue(this, key, NULL, value);
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *_(SetValue)(void *key, const Type *type, void *value) {
  KeyVal *current = MapSet_At(this, key);
  int     present = current != NULL;

  if (!current) current = NEW (KeyVal) (&this->env);

  Pair_SetF((Pair*)current, key);
  Pair_SetValueS((Pair*)current, type, value);

  if (!present) Set_Add(BASE(0), current);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *_(SetKeyValue)(const void *key, const Type *type, void *value) {
  KeyVal *current = MapSet_AtKey(this, key);

  if (!current) {
    THROW (NEW (Exception) ("Cannot set key if record doesn't already exist! (try set instead)"));
  }

  Pair_SetValueS((Pair*)current, type, value);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void _(Remove)(const void *key) {
  const KeyVal keyval = { .base.first = (void*)key };

  Set_Remove(BASE(0), &keyval);
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveKey)(const void *key) {
  const KeyVal keyval = { .base.first = (void*)key };

  Set_RemoveKey(BASE(0), &keyval);
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *CONST (At)(const void *key) {
  const KeyVal keyval = { .base.first = (void*)key };
  int          index  = Set_Contains(BASE(0), &keyval);

  return index >= 0 ? Array_At(BASE(2), index) : NULL;
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *CONST (AtKey)(const void *key) {
  const KeyVal keyval = { .base.first = (void*)key };
  int          index  = Set_ContainsKey(BASE(0), &keyval);

  return index >= 0 ? Array_At(BASE(2), index) : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAt)(const void *key) {
  Pair *p = (Pair*)MapSet_At(this, key);
  
  return p ? p->second : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtKey)(const void *key) {
  Pair *p = (Pair*)MapSet_AtKey(this, key);
  
  return p ? p->second : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtDeref)(const void *key) {
  Pair *p = (Pair*)MapSet_At(this, key);

  return p ? Pair_SDeref(p) : NULL;
}

////////////////////////////////////////////////////////////////////////////////
void *CONST (ValueAtKeyDeref)(const void *key) {
  Pair *p = (Pair*)MapSet_AtKey(this, key);

  return p ? Pair_SDeref(p) : NULL;
}

#undef TYPENAME
