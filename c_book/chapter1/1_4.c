#include <stdio.h>

int main() {
  float farh, celsius;
  int lower, upper, step;

  lower = 0;
  upper = 300;
  step = 20;

  celsius = lower;
  printf("%s %s\n", "Celsius", "Farh");
  while (celsius <= upper) {
    // celsius = (5.0 / 9.0) * (farh - 32.0);
    // celsius / (5.0 / 9.0) = (farh - 32.0);
    // celsius / (5.0 / 9.0) + 32.0 = farh;
    farh = celsius / (5.0 / 9.0) + 32.0;
    printf("%6.0f %3.1f\n", celsius, farh);
    celsius = celsius + step;
  }
  return 0;
}