#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100
#define NUMBER '0'

int getop(char s[]);
void push(double);
double pop(void);
void print_top_stack_el();
void duplicate_top_stack_el();
void swap_top_stack_el();
void clear_stack();

int is_eof = 0;


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
    case 'S':
      op2 = pop();
      push(sin(op2));
      if (is_eof) {
        is_eof = 0;
        printf("\t%.8g\n", pop());
      }
      break;
    case 'E':
      op2 = pop();
      push(exp(op2));
      if (is_eof) {
        is_eof = 0;
        printf("\t%.8g\n", pop());
      }
      break;
    case 'P':
      op2 = pop();
      op1 = pop();
      push(pow(op1, op2));
      if (is_eof) {
        is_eof = 0;
        printf("\t%.8g\n", pop());
      }
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

void print_top_stack_el() {
  if (!sp) {
    return;
  }

  if (val[sp-1]) {
    printf("Top stack element %f\n", val[sp-1]);
  }
}

void duplicate_top_stack_el() {
  if (!sp) {
    return;
  }

  if (val[sp-1]) {
    push(val[sp-1]);
  }
}

void swap_top_stack_el() {
  if (sp < 2) {
    return;
  }

  double tmp;
  tmp = val[sp-2];
  val[sp-2] = val[sp-1];
  val[sp-1] = tmp;
}

void clear_stack() {
  for (int i = 0; i < MAXVAL; i++) {
    val[i] = 0;
  }
  sp = 0;
}

#include <ctype.h>

int getch(void);
void ungetch(int);

int is_sin(char s[]) {
  if (s[0] == 's' && s[1] == 'i' && s[2] == 'n' && s[3] == '\0') {
    return 1;
  } else {
    return 0;
  }
}

int is_exp(char s[]) {
  if (s[0] == 'e' && s[1] == 'x' && s[2] == 'p' && s[3] == '\0') {
    return 1;
  } else {
    return 0;
  }
}

int is_pow(char s[]) {
  if (s[0] == 'p' && s[1] == 'o' && s[2] == 'w' && s[3] == '\0') {
    return 1;
  } else {
    return 0;
  }
}

int getop(char s[]) {
  int i, c;

  while ((s[0] = c = getch()) == ' ' || c == '\t') {

  }

  int next_c;
  i = 0;
  if (c == '-') {
    next_c = getch();
    if (isdigit(next_c)) {
      c = next_c;
      s[++i] = c;
      while (isdigit(s[++i] = c = getch())) {

      }
    }

    if (next_c == '\n') {
      is_eof = 1;
    }
  }

  if (!i) {
    i = 1;
  }

  s[i] = '\0';

  if (c >= 'a' && c <= 'z') {
    char math_op[4];
    int counter = 0;

    math_op[0] = c;
    while (1) {
      c = getch();
      if (c == '\n') {
        is_eof = 1;
      }

      if (c >= 'a' && c <= 'z') {
        counter++;
        math_op[counter] = c;
      } else {
        break;
      }

    }
    math_op[counter+1] = '\0';

    if (is_sin(math_op)) {
      return 'S';
    }

    if (is_exp(math_op)) {
      return 'E';
    }

    if (is_pow(math_op)) {
      return 'P';
    }
  }

  if (isdigit(c)) {
    while (isdigit(s[++i] = c = getch())) {

    }
  }

  if (c == '.') {
    while (isdigit(s[++i] = c = getch())) {

    }
  }

  if (!isdigit(c) && c != '.' && c != ' ') {
    return c;
  }

  s[i] = '\0';
  if (c != EOF && c != ' ') {
    ungetch(c);
  }

  return NUMBER;
}


#define BUFSIZE 100


char buf[BUFSIZE];
int bufp = 0;

int getch(void) {
  return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c) {
  if (bufp >= BUFSIZE) {
    printf("ungetch: too many characters\n");
  } else {
    buf[bufp++] = c;
  }
}