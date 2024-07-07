#include <stdio.h>

int lower(int c) {
  // if (c >= 'A' && c <= 'Z') {
  //   return c+32;
  // }
  return (c >= 'A' && c <= 'Z') ? c+32 : c;
}

int main() {
  printf("%c\n", lower('M'));
  return 0;
}