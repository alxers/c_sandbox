#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'

int getop(char s[]);
void push(double);
double pop(void);

int is_eof = 0;


int main() {
  char type;
  float op2;
  float op1;
  char s[MAXOP];

  while (scanf("%f %f %c", &op1, &op2, &type) != EOF) {
    switch(type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      printf("%f\n", op1+op2);
      break;
    case '*':
      printf("%f\n", op1*op2);
      break;
    case '-':
      printf("%f\n", op1 - op2);
      break;
    case '/':
      if (op2 != 0.0) {
        printf("%f\n", op1 / op2);
      } else {
        printf("error: zero division\n");
      }
      break;
    case '\n':
      break;
    default:
      printf("error: unknown command %s\n", s);
    }
  }
  return 0;
}

#define MAXVAL 100

int sp = 0;
double val[MAXVAL];

void push(double f) {
  if (sp < MAXVAL) {
    val[sp++] = f;
  } else {
    printf("error: stack full, can't push %g\n", f);
  }
}

double pop(void) {
  if (sp > 0) {
    return val[--sp];
  } else {
    printf("error: stack empty\n");
    return 0.0;
  }
}