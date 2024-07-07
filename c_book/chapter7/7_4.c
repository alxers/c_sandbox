#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>

void minscanf(char *fmt, ...) {
  va_list ap;
  char *p, *sval;
  int *ival;
  float *fval;
  char *cval;

  va_start(ap, fmt);

  for (p = fmt; *p; p++) {
    if (*p != '%') {
      // putchar(*p);
      continue;
    }

    switch (*++p) {
    case 'd':
      ival = va_arg(ap, int *);
      scanf("%d", ival);
      break;
    case 'f':
      fval = va_arg(ap, float *);
      scanf("%f", fval);
      break;
    case 's':
      sval = va_arg(ap, char *);
      scanf("%s", sval);
      break;
    case 'c':
      // cval = va_arg(ap, char *);
      // scanf("%c", cval);
      break;
    default:
      break;
    }
  }

  va_end(ap);
}

int main(int argc, char *argv[]) {
  int day, year;
  char monthname[20];

  minscanf("%d %s %d", &day, monthname, &year);
  printf("\n");
  printf("First %d %s %d", day, monthname, year);
  printf("\n");

  int dday, yyear, mmonth;
  minscanf("%d/%d/%d", &dday, &mmonth, &yyear);
  printf("\n");
  printf("Second %d %d %d", dday, mmonth, yyear);
  printf("\n");

}