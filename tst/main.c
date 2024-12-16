#include <stdio.h>

#include <array.h>
#include <objectarray.h>
#include <pair.h>
#include <map.h>
#include <set.h>
#include <list.h>
#include <diagnostic.h>
#include <print.h>
#include <collection.str.h>
#include <iterator.h>

void array_tests()
{
  printf("%s\n", "ARRAY TESTS:");

  CHECK_MEMORY

  Array *test = Array_From(3, sizeof(int), (int[]){ 9, 1, 43 });

  CHECK_MEMORY

  print("%[%d]O\n", test);

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

  Array *a = Array_From(3, sizeof(int), (int[]){ 1, 2, 3 });
  Array *b = Array_From(2, sizeof(int), (int[]){ 33, 44 });
  Array *c = Array_From(2, sizeof(int), (int[]){ -7, -8});

  ObjectArray *test = ObjectArray_Fill(NEW (ObjectArray)(TYPEOF(Array)), a, b, c, NULL);

  CHECK_MEMORY

  //print("%[%(Array)[%d]O]O\n", test);
  print("%O\n", test);

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
  printf("%s\n", "PAIR TESTS:");

  CHECK_MEMORY

  Array *a = Array_From(3, sizeof(int), (int[]){ 1, 2, 3 }), 
        *b = Array_From(3, sizeof(int), (int[]){ 4, 5, 6 }),
        *c = Array_From(3, sizeof(int), (int[]){ 7, 8, 9 });

  CHECK_MEMORY

  Pair *p = NEW (Pair) ();

  CHECK_MEMORY

  a = Pair_SetF(p, a);
  b = Pair_SetS(p, b);

  print("%O\n", p);

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
  printf("%s\n", "MAP TESTS:");

  CHECK_MEMORY

  Array *a = Array_From(2, sizeof(int), (int[]){ 0, 1 }), 
        *b = Array_From(2, sizeof(int), (int[]){ 1, 2 }),
        *c = Array_From(2, sizeof(int), (int[]){ 2, 3 }),
        *d = Array_From(2, sizeof(int), (int[]){ 3, 4 });

  CHECK_MEMORY

  Map *m = NEW (Map) (TYPEOF(Array));

  m->comparer = (Comparer)array_comparer;

  CHECK_MEMORY

  Pair p1 = *Map_Set(m, a, b);
  a = p1.first;
  b = p1.second;

  Pair p2 = *Map_Set(m, c, d);
  c = p2.first;
  d = p2.second;

  CHECK_MEMORY

  d = Map_ValueAt(m, c);

  for (int i = 0; i < d->size; i++)
  {
    printf("Index (d): %d\n", *(int*)Array_At(d, i));
  }

  DELETE (m);

  CHECK_MEMORY
}

void set_tests()
{
  printf("%s\n", "SET TESTS:");

  CHECK_MEMORY

  Set *test = NEW (Set) (TYPEOF (int));

  Set_Add(test, NEW (int)(0));
  Set_Add(test, NEW (int)(3));
  Set_Add(test, NEW (int)(1));
  Set_Add(test, NEW (int)(0));
  Set_Add(test, NEW (int)(2));

  print("%Of\n", test);

  CHECK_MEMORY
}

void list_tests()
{
  printf("%s\n", "LIST TESTS:");

  CHECK_MEMORY

  List *test = NEW (List)();

  int na = 1, nb = 2, nc = 3;

  List_AddValue(test, TYPEOF(int), &na);
  List_AddValue(test, TYPEOF(int), &nb);
  List_AddValue(test, TYPEOF(int), &nc);

  for (int i = 0; i < List_Size(test); i++)
  {
    printf("%d: %d\n", i, *(int*)List_At(test, i));
  }

  int *pop;

  List_Remove(test, (void**)&pop);

  printf("Popped: %d\n", *pop);

  DELETE (pop);

  for (int i = 0; i < List_Size(test); i++)
  {
    printf("%d: %d\n", i, *(int*)List_At(test, i));
  }

  List *other = List_Fill(NEW (List)(), NEW(int)(2), NEW(int)(3), NEW(int)(1), NULL);

  print("%O\n", test);
  print("%O\n", other);

  List_Merge(test, other);

  print("%O\n", test);

  List_RemoveAt(test, 1, NULL);

  print("%O\n", test);

  List_InsertValue(test, 2, TYPEOF(int), &nb);

  for (int i = 0; i < List_Size(test); i++)
  {
    printf("%d: %d\n", i, *(int*)List_At(test, i));
  }

  DELETE (test);

  CHECK_MEMORY
}

void iterate(void *collection)
{
  for (Iterator *it = NEW (Iterator)(collection); !done(it); next(it))
  {
    int *number = it->base;

    printf("%d\n", *number);
  }
}

int main(void)
{
  array_tests();
  object_array_tests();
  pair_tests();
  map_tests();
  set_tests();
  list_tests();

  List        *list  = List_Fill       (NEW (List)(),                   NEW(int)(1), NEW(int)(2), NEW(int)(3), NULL);
  ObjectArray *array = ObjectArray_Fill(NEW (ObjectArray)(TYPEOF(int)), NEW(int)(1), NEW(int)(2), NEW(int)(3), NULL);

  iterate(list);
  print("%Of\n", list);
  iterate(array);
  print("%Of\n", array);

  CHECK_MEMORY
  STOP_WATCHING

  return 0;
}