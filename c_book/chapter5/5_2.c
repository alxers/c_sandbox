#include <stdio.h>
#include <ctype.h>

int getch();
void ungetch();

int getfloat(float *pn) {
  int c, sign;

  while (isspace(c = getch()))
    ;

  if (!isdigit(c) && c != EOF && c != '+' && c != '-') {
    ungetch(c); // it's not a number
    return 0;
  }

  sign = (c == '-') ? -1 : 1;

  if (c == '+' || c == '-') {
    c = getch();

    if (!isdigit(c)) {
      ungetch(c);
      return 0;
    }
  }

  int is_dot = 0;
  int pow = 1;
  for (*pn = 0.0; isdigit(c) || c == '.'; c = getch()) {
    if (c == '.') {
      is_dot = 1;
      continue;
    }

    if (is_dot) {
      pow *= 10;
    }
    *pn = 10 * *pn + (c - '0');
  }

  *pn *= sign;
  *pn = *pn / pow;

  if (c != EOF) {
    ungetch(c);
  }

  printf("%f\n", *pn);
  return c;
}

#define SIZE 3

int main() {

  int n;
  float array[SIZE];

  for (n = 0; n < SIZE && getfloat(&array[n]) != EOF; n++)
    ;

  return 0;
}