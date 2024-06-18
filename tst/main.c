#include <stdio.h>

#include <array.h>
#include <diagnostic.h>

void array_tests()
{
  Array *test = NEW (Array) (sizeof(int));

  printf("%d\n", test->size);

  CHECK_MEMORY

  DELETE (test);
}

int main(void)
{
  CHECK_MEMORY

  array_tests();

  CHECK_MEMORY

  STOP_WATCHING

  return 0;
}