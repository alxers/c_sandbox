#include <stdio.h>
#include <string.h>
#include <limits.h>

void reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

// Better option
void itoa(int n, char s[], int width) {
  int i, sign = 0;

  i = 0;
  int num;

  do {
    num = n % 10;
    if (num < 0) {
      sign = -1;
      num = -num;
    }
    s[i++] = num + '0';
  } while ((n /= 10) != 0);

  if (sign < 0) {
    s[i++]  = '-';
  }

  int diff = width - i;
  if (diff > 0) {
    for (int a = 0; a < diff; a++) {
      s[i++] = ' ';
    }
  }
  s[i] = '\0';
  reverse(s);
}


int main() {

  char s[100];
  itoa(-2147483648, s, 50);
  // itoa(-2147483647, s);
  // itoa(-100, s);
  // itoa(2147483647, s, 5);
  // itoa(0, s);
  // itoa(123, s, 5);
  printf("%s\n", s);
  return 0;
}