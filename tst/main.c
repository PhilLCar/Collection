#include <stdio.h>

#include <array.h>
#include <objectarray.h>
#include <pair.h>
#include <map.h>
#include <diagnostic.h>

void array_tests()
{
  printf("%s\n", "ARRAY TESTS:");

  CHECK_MEMORY

  Array *test = NEW (Array) (sizeof(int));

  CHECK_MEMORY

  int a = 9, b = 1, c = 43;

  Array_push(test, &a);
  Array_push(test, &b);
  Array_push(test, &c);

  printf("Size: %d\n", test->size);

  printf("Elem 2: %d\n", *(int*)Array_at(test, 2));

  while (test->size)
  {
    printf("Pop: %d\n", *(int*)Array_pop(test));
  }

  CHECK_MEMORY

  DELETE (test);

  CHECK_MEMORY
}

void object_array_tests()
{
  printf("%s\n", "OBJECT ARRAY TESTS:");

  CHECK_MEMORY

  ObjectArray *test = NEW (ObjectArray) (OBJECT_TYPE(Array));

  CHECK_MEMORY

  Array *a = NEW (Array) (sizeof(int)), *b = NEW (Array) (sizeof(int)), *c = NEW (Array) (sizeof(int));

  int x = 1, y = 2, z = 4;

  Array_fill(a, &x, &y, &z, NULL);
  Array_fill(b, &x, &z, NULL);
  Array_fill(c, &y, &z, NULL);

  ObjectArray_fill(test, a, b, c, NULL);

  CHECK_MEMORY

  Array *tb = Array_at((Array*)test, 1);

  while (tb->size)
  {
    printf("Pop (b): %d\n", *(int*)Array_pop(tb));
  }

  ObjectArray_rem(test, 1, 0);

  printf("Size: %d\n", test->base.size);

  Array *ta = Array_at((Array*)test, 0);
  while (ta->size)
  {
    printf("Pop (a): %d\n", *(int*)Array_pop(ta));
  }

  Array *tc = Array_at((Array*)test, 1);
  while (tc->size)
  {
    printf("Pop (c): %d\n", *(int*)Array_pop(tc));
  }

  DELETE (test);

  CHECK_MEMORY
}

void pair_tests()
{
  int x = 1, y = 2, z =3;

  printf("%s\n", "PAIR TESTS:");

  CHECK_MEMORY

  Array *a = Array_fill(NEW (Array) (sizeof(int)), &x, &y, NULL), 
        *b = Array_fill(NEW (Array) (sizeof(int)), &x, &z, NULL),
        *c = Array_fill(NEW (Array) (sizeof(int)), &y, &z, NULL);

  CHECK_MEMORY

  Pair *p = NEW (Pair) (OBJECT_TYPE(Array), OBJECT_TYPE(Array));

  CHECK_MEMORY

  a = Pair_setf(p, a);
  b = Pair_sets(p, b);

  printf("Pop (a): %d\n", *(int*)Array_pop(a));
  printf("Rem (b): %d\n", *(int*)Array_rem(b, 0));

  c = Pair_setf(p, c);

  CHECK_MEMORY

  DELETE (p);

  CHECK_MEMORY
}

int array_comparer(Array *a, Array *b)
{
  int a1 = *(int*)Array_at(a, 1);
  int b1 = *(int*)Array_at(b, 1);

  return a1 == b1;
}

void map_tests()
{
  int w = 0, x = 1, y = 2, z =3;

  printf("%s\n", "MAP TESTS:");

  CHECK_MEMORY

  Array *a = Array_fill(NEW (Array) (sizeof(int)), &w, &x, NULL), 
        *b = Array_fill(NEW (Array) (sizeof(int)), &x, &y, NULL),
        *c = Array_fill(NEW (Array) (sizeof(int)), &y, &z, NULL),
        *d = Array_fill(NEW (Array) (sizeof(int)), &z, &w, NULL);

  CHECK_MEMORY

  Map *m = NEW (Map) (OBJECT_TYPE(Array), OBJECT_TYPE(Array), (Comparer)array_comparer);

  CHECK_MEMORY

  Pair p1 = *Map_setkey(m, a, b);
  a = p1.first.object;
  b = p1.second.object;

  Pair p2 = *Map_setkey(m, c, d);
  c = p2.first.object;
  d = p2.second.object;

  CHECK_MEMORY

  d = Map_vatkey(m, c);

  for (int i = 0; i < d->size; i++)
  {
    printf("Index (d): %d\n", *(int*)Array_at(d, i));
  }

  DELETE (m);

  CHECK_MEMORY
}

int main(void)
{
  array_tests();
  object_array_tests();
  pair_tests();
  map_tests();

  STOP_WATCHING

  return 0;
}