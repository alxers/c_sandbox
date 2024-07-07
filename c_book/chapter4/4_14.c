#include <stdio.h>

#define swap(t, x, y)  { t tmp = x; x = y; y = tmp; }

int main() {

  float a = 1;
  float b = 2;

  printf("Before, a = %f, b = %d\n", a, b);

  swap(float, a, b);

  printf("After, a = %d, b = %f\n", a, b);

  return 0;
}