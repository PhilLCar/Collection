#include <comparer.h>

#define COMPARER(N) int _default_##N##_comparer(const void *against, const void *reference)\
{\
  return memcmp(reference, against, N);\
}

#define COMPREF(N) _default_##N##_comparer

COMPARER(1)
COMPARER(2)
COMPARER(3)
COMPARER(4)
COMPARER(5)
COMPARER(6)
COMPARER(7)
COMPARER(8)
COMPARER(9)
COMPARER(10)
COMPARER(11)
COMPARER(12)
COMPARER(13)
COMPARER(14)
COMPARER(15)
COMPARER(16)

Comparer _default_comparers[17] = {
  NULL,
  COMPREF(1),
  COMPREF(2),
  COMPREF(3),
  COMPREF(4),
  COMPREF(5),
  COMPREF(6),
  COMPREF(7),
  COMPREF(8),
  COMPREF(9),
  COMPREF(10),
  COMPREF(11),
  COMPREF(12),
  COMPREF(13),
  COMPREF(14),
  COMPREF(15),
  COMPREF(16),
};

int _default_comparer(const void *against, const void *reference) {
  // simply compare pointers;
  return reference - against;
}

int _default_object_comparer(const void *against, const void *reference)
{
  const Type *type = gettype(reference);

  return memcmp(against, reference, type->size);
}

int _default_key_comparer(const void **against, const void *reference)
{
  return *against - reference;
}

Comparer comparer(const Type *type) {
  Comparer comparer = (Comparer)virtual(type, "Comparer");

  if (!comparer) {
    if (isobject(type)) {
      comparer = _default_object_comparer;
    } else if (type->size <= (sizeof(_default_comparers) / sizeof(Comparer))) {
      comparer = _default_comparers[type->size];
    } else {
      THROW (NEW (Exception) ("A default comparer is not implemented for native types of size %ld", type->size));
    }
  }

  return comparer;
}

Comparer key_comparer(const Type *type) {
  return (Comparer)IFNULL(virtual(type, "KeyComparer"), comparer(type));
}