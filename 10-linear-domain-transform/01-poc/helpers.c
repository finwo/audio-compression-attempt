#include <stdlib.h>

void swap(void* pa, void* pb, size_t sz)
{
  char *p1 = pa, *p2 = pb;
  while (sz--)
  {
    char tmp = *p1;
    *p1++ = *p2;
    *p2++ = tmp;
  }
}

void interleave(char* s, size_t len)
{
  size_t start, step, i, j;

  if (len <= 2)
    return;

  if (len & (len - 1))
    return; // only power of 2 lengths are supported

  for (start = 1, step = 2;
       step < len;
       start *= 2, step *= 2)
  {
    for (i = start, j = len / 2;
         i < len / 2;
         i += step, j += step)
    {
      swap(s + i,
           s + j,
           step / 2);
    }
  }
}
