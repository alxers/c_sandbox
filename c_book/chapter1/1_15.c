#include <stdio.h>

int far_to_cel() {
  int step = 20;
  int max = 200;
  for (int i = 0; i <= max; i+=step) {
    printf("%6d %6.1f\n", i, (5.0/9.0) * (i - 32.0));
  }
  return 0;
}

int main() {

  far_to_cel();

  return 0;
}