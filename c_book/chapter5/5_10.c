#include <stdio.h>
#include <stdlib.h>

#define MAXOP 100
#define NUMBER '0'

void push(double);
double pop(void);

int is_eof = 0;


int main(int argc, char *argv[]) {
  int type;
  double op2;
  double op1;
  // char s[MAXOP];
  argv++;
  argc--;

  while (argc-- > 0) {
    type = **argv;
    int is_a_digit = isdigit(type);
    // Example
    // char *a[] = { "5" };
    // int test = isdigit('5');
    // int test2 = isdigit(**a);
    // 
    // If we use int test2 = isdigit(*a); it'll give "5", thus isdigit("5") == 0 (not a digit)
    // but **a gives '5' (???) and isdigit('5') is a non zero value - so it's a digit
    // end Example
    if (is_a_digit) {
      type = NUMBER;
    }
    switch(type) {
    case NUMBER:
      push(atof(*argv));
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
      printf("error: unknown command %s\n", *argv);
    }
    argv++;
  }

  if (argc <= 0) {
      printf("\t%.8g\n", pop());
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


// Echo

// #include <stdio.h>

// int main(int argc, char *argv[]) {
//   int i;

//   for (i = 1; i < argc; i++) {
//     printf("%s%s", argv[i], (i < argc-1) ? " " : "");
//   }
//   printf("\n");
//   return 0;
// }