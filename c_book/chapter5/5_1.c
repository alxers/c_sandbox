#include <stdio.h>
#include <ctype.h>

int getch();
void ungetch();

int getint(int *pn) {
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

  for (*pn = 0; isdigit(c); c = getch()) {
    *pn = 10 * *pn + (c - '0');
  }

  *pn *= sign;

  if (c != EOF) {
    ungetch(c);
  }

  printf("%d\n", *pn);
  return c;
}

#define SIZE 3

int main() {

  int n, array[SIZE];

  for (n = 0; n < SIZE && getint(&array[n]) != EOF; n++)
    ;

  return 0;
}