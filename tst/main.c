#include <stdio.h>

#include <array.h>
#include <objectarray.h>
#include <pair.h>
#include <map.h>
#include <set.h>
#include <diagnostic.h>

void array_tests()
{
  printf("%s\n", "ARRAY TESTS:");

  CHECK_MEMORY

  Array *test = NEW (Array) (sizeof(int));

  CHECK_MEMORY

  int a = 9, b = 1, c = 43;

  Array_Push(test, &a);
  Array_Push(test, &b);
  Array_Push(test, &c);

  printf("Size: %d\n", test->size);

  printf("Elem 2: %d\n", *(int*)Array_At(test, 2));

  while (test->size)
  {
    printf("Pop: %d\n", *(int*)Array_Pop(test));
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

  Array_Fill(a, &x, &y, &z, NULL);
  Array_Fill(b, &x, &z, NULL);
  Array_Fill(c, &y, &z, NULL);

  ObjectArray_Fill(test, a, b, c, NULL);

  CHECK_MEMORY

  Array *tb = Array_At((Array*)test, 1);

  while (tb->size)
  {
    printf("Pop (b): %d\n", *(int*)Array_Pop(tb));
  }

  ObjectArray_RemoveAt(test, 1, 0);

  printf("Size: %d\n", test->base.size);

  Array *ta = Array_At((Array*)test, 0);
  while (ta->size)
  {
    printf("Pop (a): %d\n", *(int*)Array_Pop(ta));
  }

  Array *tc = Array_At((Array*)test, 1);
  while (tc->size)
  {
    printf("Pop (c): %d\n", *(int*)Array_Pop(tc));
  }

  DELETE (test);

  CHECK_MEMORY
}

void pair_tests()
{
  int x = 1, y = 2, z =3;

  printf("%s\n", "PAIR TESTS:");

  CHECK_MEMORY

  Array *a = Array_Fill(NEW (Array) (sizeof(int)), &x, &y, NULL), 
        *b = Array_Fill(NEW (Array) (sizeof(int)), &x, &z, NULL),
        *c = Array_Fill(NEW (Array) (sizeof(int)), &y, &z, NULL);

  CHECK_MEMORY

  Pair *p = NEW (Pair) (OBJECT_TYPE(Array), OBJECT_TYPE(Array));

  CHECK_MEMORY

  a = Pair_SetF(p, a);
  b = Pair_SetS(p, b);

  printf("Pop (a): %d\n", *(int*)Array_Pop(a));
  printf("Rem (b): %d\n", *(int*)Array_Remove(b));

  c = Pair_SetF(p, c);

  CHECK_MEMORY

  DELETE (p);

  CHECK_MEMORY
}

int array_comparer(Array *a, Array *b)
{
  int a1 = *(int*)Array_At(a, 1);
  int b1 = *(int*)Array_At(b, 1);

  return a1 != b1;
}

void map_tests()
{
  int w = 0, x = 1, y = 2, z =3;

  printf("%s\n", "MAP TESTS:");

  CHECK_MEMORY

  Array *a = Array_Fill(NEW (Array) (sizeof(int)), &w, &x, NULL), 
        *b = Array_Fill(NEW (Array) (sizeof(int)), &x, &y, NULL),
        *c = Array_Fill(NEW (Array) (sizeof(int)), &y, &z, NULL),
        *d = Array_Fill(NEW (Array) (sizeof(int)), &z, &w, NULL);

  CHECK_MEMORY

  Map *m = NEW (Map) (OBJECT_TYPE(Array), OBJECT_TYPE(Array), (Comparer)array_comparer);

  CHECK_MEMORY

  Pair p1 = *Map_Set(m, a, b);
  a = p1.first.object;
  b = p1.second.object;

  Pair p2 = *Map_Set(m, c, d);
  c = p2.first.object;
  d = p2.second.object;

  CHECK_MEMORY

  d = Map_ValueAt(m, c);

  for (int i = 0; i < d->size; i++)
  {
    printf("Index (d): %d\n", *(int*)Array_At(d, i));
  }

  DELETE (m);

  CHECK_MEMORY
}

int cmp(int *a, int *b)
{
  return *b - *a;
}

void set_tests()
{
  printf("%s\n", "SET TESTS:");

  CHECK_MEMORY

  Set *test = NEW (Set) (NATIVE_TYPE(int), (Comparer)cmp);

  int add = 0;
  Set_Add(test, &add);
  add = 3;
  Set_Add(test, &add);
  add = 1;
  Set_Add(test, &add);
  add = 0;
  Set_Add(test, &add);
  add = 2;
  Set_Add(test, &add);

  for (int i = 0; i < ((Array*)test)->size; i++) {
    printf("%d : %d\n", i, *(int*)Array_At((Array*)test, i));
  }

  DELETE (test);

  CHECK_MEMORY
}

int main(void)
{
  array_tests();
  object_array_tests();
  pair_tests();
  map_tests();
  set_tests();

  STOP_WATCHING

  return 0;
}