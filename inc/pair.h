#ifndef PAIR_H
#define PAIR_H

#define PAIR(TYPE1, TYPE2) \
struct {\
  TYPE1 first;\
  TYPE2 second;\
}

#define PAIR_PTR(SIZE1, SIZE2) \
struct {\
  char first[SIZE1];\
  char second[SIZE2];\
}

#define FIRST(TYPE1, PAIR_PTR) ((TYPE1*)PAIR)
#define SECOND(TYPE1, TYPE2, PAIR_PTR) ((TYPE2*)((char*)PAIR + sizeof(TYPE1)))

#endif