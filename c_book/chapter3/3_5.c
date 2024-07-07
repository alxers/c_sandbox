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

int hex_val(int c) {
  int result = c;
  if (c > 9 && c < 16) {
    switch(c) {
      case 15:
        result = 'F';
        break;
      case 14:
        result = 'E';
        break;
      case 13:
        result = 'D';
        break;
      case 12:
        result = 'C';
        break;
      case 11:
        result = 'B';
        break;
      case 10:
        result = 'A';
        break;
      default:
        break;
    }
  }

  return result;
}

void itob(int n, char s[], int b) {
  int i = 0;
  int sign = 0;
  int num;

  if (b == 16 && n < 16) {
    s[i++] = hex_val(n);
    if (i > 0) {
      s[i] = '\0';
      return;
    }
  }

  do {
    num = n % b;
    if (num < 0) {
      sign = -1;
      num = -num;
    }
    if (b == 16 && num > 9 && num < 16) {
      num = hex_val(num) - '0';
    }
    s[i++] = num + '0';
  } while ((n /= b) != 0);

  if (sign < 0) {
    s[i++]  = '-';
  }
  s[i] = '\0';
  reverse(s);
}


int main() {
  char s[100];
  // itob(-2147483648, s, 10);
  // itob(127, s, 8);
  // itob(127, s, 16);
  // itob(11, s, 16);
  itob(5, s, 2);
  printf("%s\n", s);
  return 0;
}