#include <stdio.h>
#include <stdlib.h>
#include "calc.h"
#include "is_eof.h"

#define MAXOP 100

is_eof = 0;

int main() {
  int type;
  double op2;
  double op1;
  char s[MAXOP];

  while ((type = getop(s)) != EOF) {
    switch(type) {
    case NUMBER:
      push(atof(s));
      break;
    case '+':
      op2 = pop();
      op1 = pop();
      push(op1 + op2);
      break;
    case '*':
      push(pop() * pop());
      break;
    case '-':
      op2 = pop();
      op1 = pop();
      push(op1 - op2);
      if (is_eof) {
        is_eof = 0;
        printf("\t%.8g\n", pop());
      }
      break;
    case '/':
      op2 = pop();
      if (op2 != 0.0) {
        push(pop() / op2);
      } else {
        printf("error: zero division\n");
      }
      break;
    case '%':
      op2 = pop();
      op1 = pop();
      int result = ((int) op1) % ((int) op2);
      push((double) result);
      break;
    case '\n':
      printf("\t%.8g\n", pop());
      break;
    default:
      printf("error: unknown command %s\n", s);
    }
  }
  return 0;
}