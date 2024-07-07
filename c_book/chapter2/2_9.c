#include <stdio.h>

int bitcount(unsigned x) {
  int b;
  int interations = 0;

  for (b = 0; x != 0; x >>= 1) {
    interations++;
    if (x & 01) {
      b++;
    }
  }

  printf("iterations bitcount %d\n", interations);
  return b;
}

int bitcount2(unsigned x) {
  int b;

  for (b = 0; x != 0; x &= (x-1)) {
    b++;
  }

  printf("iterations bitcount2 %d\n", b);
  return b;
}

int main() {
  // x rightmost bit can be either 1 or 0, when we substract 1 from x
  // we invert the rightmost bit.
  // So we always get x rightmost and x-1 righmost bits inverted
  // when we apply & to them it's always going to be 0.

  printf("29475987 has %d bits\n", bitcount(29475987));
  printf("29475987 has %d bits (2)\n", bitcount2(29475987));

  printf("0 has %d bits\n", bitcount(0));
  printf("0 has %d bits (2)\n", bitcount2(0));

  printf("6 has %d bits\n", bitcount(6));
  printf("6 has %d bits (2)\n", bitcount2(6));

  printf("max unsigned has %d bits\n", bitcount(~0));
  printf("max unsigned has %d bits (2)\n", bitcount2(~0));

  printf("31 unsigned has %d bits\n", bitcount(31));
  printf("31 unsigned has %d bits (2)\n", bitcount2(31));

  printf("32 unsigned has %d bits\n", bitcount(32));
  printf("32 unsigned has %d bits (2)\n", bitcount2(32));
  return 0;
}