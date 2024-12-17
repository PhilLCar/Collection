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
    ObjectArray_Destruct(BASE(0));
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
  Pair *current = IFNULL(MapSet_At(this, key), ObjectArray_Push(BASE(1), NEW (KeyVal) (&this->env)));

  Pair_SetF(current, key);
  Pair_SetValueS(current, type, value);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *_(SetKeyValue)(const void *key, const Type *type, void *value) {
  Pair *current = (Pair*)MapSet_AtKey(this, key);

  if (!current) {
    THROW (NEW (Exception) ("Cannot set key if record doesn't already exist! (try set instead)"));
  }

  Pair_SetValueS(current, type, value);

  return current;
}

////////////////////////////////////////////////////////////////////////////////
void _(Remove)(const void *key) {
  Set_Remove(BASE(0), key, 0);
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveKey)(const void *key) {
  Set_RemoveKey(BASE(0), key, 0);
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *CONST (At)(const void *key) {
  return Array_At(BASE(2), Set_Contains(BASE(0), key));
}

////////////////////////////////////////////////////////////////////////////////
KeyVal *CONST (AtKey)(const void *key) {
  return Array_At(BASE(2), Set_ContainsKey(BASE(0), key));
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
