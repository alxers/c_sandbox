#include <stdio.h>

int main() {
  // for (int farh = 0; farh <= 300; farh = farh + 20) {
  //   printf("%3d %6.1f\n", farh, (5.0 / 9.0) * (farh - 32.0));
  // }

  for (int farh = 300; farh >= 0; farh = farh - 20) {
    printf("%3d %6.1f\n", farh, (5.0 / 9.0) * (farh - 32.0));
  }
  return 0;
}