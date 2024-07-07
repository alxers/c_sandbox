#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXOP 100
#define NUMBER '0'

int getop(char s[]);
void push(double);
double pop(void);
double peek_stack();
void print_top_stack_el();
void duplicate_top_stack_el();
void swap_top_stack_el();
void clear_stack();

int is_eof = 0;
int var_char;


int main() {
  int type;
  double op2;
  double op1;
  char st[MAXOP];

  double a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, j = 0, k = 0, l = 0, m = 0,
         n = 0, o = 0, p = 0, q = 0, r = 0, s = 0, t = 0, u = 0, v = 0, w = 0, x = 0, y = 0, z = 0;

  while ((type = getop(st)) != EOF) {
    switch(type) {
    case NUMBER:
      push(atof(st));
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
    // Process variables
    case 'a':
      if (!a) {
        a = pop();
      } else {
        push(a);
      }
      break;
    case 'b':
      if (!b) {
        b = pop();
      } else {
        push(b);
      }
      break;
    case 'c':
      if (!c) {
        c = pop();
      } else {
        push(c);
      }
      break;
    case 'd':
      if (!d) {
        d = pop();
      } else {
        push(d);
      }
      break;
    case 'e':
      if (!e) {
        e = pop();
      } else {
        push(e);
      }
      break;
    case 'f':
      if (!f) {
        f = pop();
      } else {
        push(f);
      }
      break;
    case 'g':
      if (!g) {
        g = pop();
      } else {
        push(g);
      }
      break;
    case 'h':
      if (!h) {
        h = pop();
      } else {
        push(h);
      }
      break;
    case 'i':
      if (!i) {
        i = pop();
      } else {
        push(i);
      }
      break;
    case 'j':
      if (!j) {
        j = pop();
      } else {
        push(j);
      }
      break;
    case 'k':
      if (!k) {
        k = pop();
      } else {
        push(k);
      }
      break;
    case 'l':
      if (!l) {
        l = pop();
      } else {
        push(l);
      }
      break;
    case 'm':
      if (!m) {
        m = pop();
      } else {
        push(m);
      }
      break;
    case 'n':
      if (!n) {
        n = pop();
      } else {
        push(n);
      }
      break;
    case 'o':
      if (!o) {
        o = pop();
      } else {
        push(o);
      }
      break;
    case 'p':
      if (!p) {
        p = pop();
      } else {
        push(p);
      }
      break;
    case 'q':
      if (!q) {
        q = pop();
      } else {
        push(q);
      }
      break;
    case 'r':
      if (!r) {
        r = pop();
      } else {
        push(r);
      }
      break;
    case 's':
      if (!s) {
        s = pop();
      } else {
        push(s);
      }
      break;
    case 't':
      if (!t) {
        t = pop();
      } else {
        push(t);
      }
      break;
    case 'u':
      if (!u) {
        u = pop();
      } else {
        push(u);
      }
      break;
    case 'v':
      if (!v) {
        v = pop();
      } else {
        push(v);
      }
      break;
    case 'w':
      if (!w) {
        w = pop();
      } else {
        push(w);
      }
      break;
    case 'x':
      if (!x) {
        x = pop();
      } else {
        push(x);
      }
      break;
    case 'y':
      if (!y) {
        y = pop();
      } else {
        push(y);
      }
      break;
    case 'z':
      if (z) {
        push(z);
      }
      break;
    case '\n':
      z = pop();
      printf("\t%.8g\n", z);
      break;
    case '$':
      printf("EOF\n");
      break;
    default:
      printf("error: unknown command %s\n", st);
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

double peek_stack() {
  return val[sp-1];
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
void ungets(char s[]);

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
    int is_var = 1;
    var_char = c;

    char math_op[4];
    int counter = 0;

    math_op[0] = c;
    while (1) {
      c = getch();
      if (c == '\n') {
        is_eof = 1;
      }

      if (c >= 'a' && c <= 'z') {
        is_var = 0;
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

    if (is_var) {
      return var_char;
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
    // This doesn't seem to be called ever (even in original programm)
    // ungetch(c);
    ungets(s);
  }

  return NUMBER;
}

int buff = 0;

int getch(void) {
  if (buff && buff == EOF) {
    return EOF;
  } else if (buff) {
    return buff;
  } else {
    int tmp = getchar();
    if (tmp == EOF) {
      return '$';
    } else {
      return tmp;
    }
  }
}

void ungetch(int c) {
  if (!buff) {
    printf("ungetch: nothing to return\n");
  } else {
    buff = c;
  }
}

void ungets(char s[]) {
  for (int i = 0; s[i] != '\0'; i++) {
    putchar(s[i]);
    ungetch(s[i]);
  }
}