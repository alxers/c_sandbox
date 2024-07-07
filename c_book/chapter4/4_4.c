#include <stdio.h>
#include <stdlib.h>

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
    print_top_stack_el();
    clear_stack();
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