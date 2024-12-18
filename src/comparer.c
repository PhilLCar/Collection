#include <comparer.h>

// TODO: Make different comparers for signed, unsigned and float types

#define COMPARER(N) int _default_##N##_comparer(const void *against, const void *reference)\
{\
  return memcmp(reference, against, N);\
}

#define COMPREF(N) _default_##N##_comparer

#define CMP(TYPE) int TYPE##_comparer(const void *against, const void *reference)\
{\
  return *(TYPE*)reference - *(TYPE*)against;\
}

#define CMPREF(TYPE) TYPE##_comparer

// Signed integers
CMP(__int8_t)
CMP(__int16_t)
CMP(__int32_t)
CMP(__int64_t)
CMP(__int128_t)

Comparer _signed_comparers[6] = {
  NULL,
  CMPREF(__int8_t),
  CMPREF(__int16_t),
  CMPREF(__int32_t),
  CMPREF(__int64_t),
  CMPREF(__int128_t)
};

// Unsigned integers
CMP(__uint8_t)
CMP(__uint16_t)
CMP(__uint32_t)
CMP(__uint64_t)
CMP(__uint128_t)

Comparer _unsigned_comparers[6] = {
  NULL,
  CMPREF(__uint8_t),
  CMPREF(__uint16_t),
  CMPREF(__uint32_t),
  CMPREF(__uint64_t),
  CMPREF(__uint128_t)
};

// Floats
//CMP(_Float16)
CMP(_Float32)
CMP(_Float64)
CMP(__float128)

Comparer _float_comparers[6] = {
  NULL,
  NULL,
  NULL,
  CMPREF(_Float32),
  CMPREF(_Float64),
  CMPREF(__float128)
};

// Default (memory)
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

int _default_pointer_comparer(const void *against, const void *reference) {
  // simply compare pointers;
  return reference - against;
}

int _default_key_comparer(const void **against, const void *reference)
{
  // Compare the pointer at root of object/struct with reference
  return *against - reference;
}

int _default_object_comparer(const void *against, const void *reference)
{
  const Type *type = gettype(reference);

  return memcmp(against, reference, type->size);
}

Comparer comparer(const Type *type) {
  Comparer comparer = (Comparer)virtual(type, "Comparer");

  if (!comparer) {
    switch (type->category) {
      case TYPES_FLOAT:
        comparer = _float_comparers[type->size];
        break;
      case TYPES_SIGNED:
        comparer = _signed_comparers[type->size];
        break;
      case TYPES_UNSIGNED:
        comparer = _unsigned_comparers[type->size];
        break;
      case TYPES_OBJECT:
        comparer = _default_object_comparer;
        break;
      case TYPES_POINTER:
        comparer = _default_pointer_comparer;
        break;
      case TYPES_DEFAULT:
        default:
        comparer = _default_comparers[type->size];
        break;
    }
  }

  return comparer;
}

Comparer key_comparer(const Type *type) {
  return (Comparer)IFNULL(virtual(type, "KeyComparer"), _default_key_comparer);
}