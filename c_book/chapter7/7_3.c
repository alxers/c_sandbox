#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

void minprintf(char *fmt, ...) {
  va_list ap;
  char *p, *sval;
  int ival;
  double dval;
  char cval;

  va_start(ap, fmt);

  for (p = fmt; *p; p++) {
    if (*p != '%') {
      putchar(*p);
      continue;
    }

    switch (*++p) {
    case 'd':
      ival = va_arg(ap, int);
      printf("%d", ival);
      break;
    case 'f':
      dval = va_arg(ap, double);
      printf("%f", dval);
      break;
    case 's':
      for (sval = va_arg(ap, char*); *sval; sval++) {
        putchar(*sval);
      }
      break;
    case 'c':
      cval = va_arg(ap, char);
      putchar(cval);
      break;
    case '%':
      putchar('%');
      break;
    default:
      putchar(*sval);
      break;
    }
  }

  va_end(ap);
}

int main(int argc, char *argv[]) {
  int c;
  while ((c = getchar()) != EOF) {
    minprintf("%c", c);
  }

  char *s = "test";
  char p = '%';
  minprintf("%s", s);
  minprintf("%%", p);
  printf("\n");
}