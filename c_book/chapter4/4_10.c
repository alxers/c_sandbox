#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h> // isdigit()

#define MAXOP 10
#define NUMBER '0'

int getop(char s[]);
void push(double);
double pop(void);
double peek_stack();
void print_top_stack_el();
void duplicate_top_stack_el();
void swap_top_stack_el();
void clear_stack();
int getline();

int is_eof = 0;
int var_char;


int main() {
  int type;
  double op2;
  double op1;
  char st[MAXOP];

  double a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0, i = 0, j = 0, k = 0, l = 0, m = 0,
         n = 0, o = 0, p = 0, q = 0, r = 0, s = 0, t = 0, u = 0, v = 0, w = 0, x = 0, y = 0, z = 0;


  char user_input_str[MAXOP];
  int digit_counter = 0;
  int counter = 0;

  int is_dot = 0;

  int result;
  result = getline(user_input_str);
  int curr_char;

  while (result != EOF) {
    curr_char = user_input_str[counter];

    if (curr_char == '\n') {
      z = pop();
      printf("\t%.8g\n", z);
      for (int i = 0; i < counter; i++) {
        user_input_str[i] = 0;
      }
      user_input_str[0] = '\0';
      counter = 0;
      result = getline(user_input_str);
      continue;
    }

    if (isdigit(curr_char)) {
      st[digit_counter] = curr_char;
      digit_counter++;

      while (1) {
        curr_char = user_input_str[counter+1];
        if (isdigit(curr_char) || curr_char == '.') {
          st[digit_counter] = curr_char;
          counter++;
          digit_counter++;
        } else {
          break;
        }
      }
      push(atof(st));
      for (int i = 0; i < MAXOP; i++) {
        st[i] = 0;
      }
      st[0] = '\0';
      digit_counter = 0;
    }

    if (curr_char == '-' && isdigit(user_input_str[counter+1])) {
      st[digit_counter] = curr_char;
      digit_counter++;

      while (1) {
        curr_char = user_input_str[counter+1];
        if (isdigit(curr_char) || curr_char == '.') {
          st[digit_counter] = curr_char;
          counter++;
          digit_counter++;
        } else {
          break;
        }
      }
      push(atof(st));
      for (int i = 0; i < MAXOP; i++) {
        st[i] = 0;
      }
      st[0] = '\0';
      digit_counter = 0;
    }

    if (curr_char >= 'a' && curr_char <= 'z') {
      if (curr_char == 's' && user_input_str[counter+1] == 'i' && user_input_str[counter+2] == 'n') {
        op2 = pop();
        push(sin(op2));
        counter = counter + 3;
        continue;
      }

      if (curr_char == 'e' && user_input_str[counter+1] == 'x' && user_input_str[counter+2] == 'p') {
        op2 = pop();
        push(exp(op2));
        counter = counter + 3;
        continue;
      }

      if (curr_char == 'p' && user_input_str[counter+1] == 'o' && user_input_str[counter+2] == 'w') {
        op2 = pop();
        op1 = pop();
        push(pow(op1, op2));
        counter = counter + 3;
        continue;
      }

      if (user_input_str[counter+1] == ' ' || user_input_str[counter+1] == '\n') {
        switch (curr_char) {
        case 'a':
          if (!a) {
            a = peek_stack();
          } else {
            push(a);
          }
          break;
        case 'b':
          if (!b) {
            b = peek_stack();
          } else {
            push(b);
          }
          break;
        case 'c':
          if (!c) {
            c = peek_stack();
          } else {
            push(c);
          }
          break;
        case 'd':
          if (!d) {
            d = peek_stack();
          } else {
            push(d);
          }
          break;
        case 'e':
          if (!e) {
            e = peek_stack();
          } else {
            push(e);
          }
          break;
        case 'f':
          if (!f) {
            f = peek_stack();
          } else {
            push(f);
          }
          break;
        case 'g':
          if (!g) {
            g = peek_stack();
          } else {
            push(g);
          }
          break;
        case 'h':
          if (!h) {
            h = peek_stack();
          } else {
            push(h);
          }
          break;
        case 'i':
          if (!i) {
            i = peek_stack();
          } else {
            push(i);
          }
          break;
        case 'j':
          if (!j) {
            j = peek_stack();
          } else {
            push(j);
          }
          break;
        case 'k':
          if (!k) {
            k = peek_stack();
          } else {
            push(k);
          }
          break;
        case 'l':
          if (!l) {
            l = peek_stack();
          } else {
            push(l);
          }
          break;
        case 'm':
          if (!m) {
            m = peek_stack();
          } else {
            push(m);
          }
          break;
        case 'n':
          if (!n) {
            n = peek_stack();
          } else {
            push(n);
          }
          break;
        case 'o':
          if (!o) {
            o = peek_stack();
          } else {
            push(o);
          }
          break;
        case 'p':
          if (!p) {
            p = peek_stack();
          } else {
            push(p);
          }
          break;
        case 'q':
          if (!q) {
            q = peek_stack();
          } else {
            push(q);
          }
          break;
        case 'r':
          if (!r) {
            r = peek_stack();
          } else {
            push(r);
          }
          break;
        case 's':
          if (!s) {
            s = peek_stack();
          } else {
            push(s);
          }
          break;
        case 't':
          if (!t) {
            t = peek_stack();
          } else {
            push(t);
          }
          break;
        case 'u':
          if (!u) {
            u = peek_stack();
          } else {
            push(u);
          }
          break;
        case 'v':
          if (!v) {
            v = peek_stack();
          } else {
            push(v);
          }
          break;
        case 'w':
          if (!w) {
            w = peek_stack();
          } else {
            push(w);
          }
          break;
        case 'x':
          if (!x) {
            x = peek_stack();
          } else {
            push(x);
          }
          break;
        case 'y':
          if (!y) {
            y = peek_stack();
          } else {
            push(y);
          }
          break;
        case 'z':
          if (z) {
            push(z);
          }
          break;
        // case '\n':
        //   z = pop();
        //   printf("\t%.8g\n", z);
        //   break;
        // case '$':
        //   printf("EOF\n");
        //   break;
        default:
          printf("error: unknown command %s\n", st);
        }
      }
    } else if (!isdigit(curr_char) && curr_char != '.' && curr_char != ' ') {
      switch(curr_char) {
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
        // if (is_eof) {
        //   is_eof = 0;
        //   printf("\t%.8g\n", pop());
        // }
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
      default:
        printf("error: unknown command %s\n", st);
      }
    }

    for (int i = 0; i < MAXOP; i++) {
      st[i] = 0;
    }
    st[0] = '\0';
    counter++;
  }
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

int getline(char s[]) {
  int c, i;

  for (i = 0; (c = getchar()) != EOF && c != '\n'; i++) {
    // Continue iterating over i, but only fill in an array
    // if it's less than global line limit
    // Thus "i" will contain the amount of characters from input (with no limits)
    if (i < MAXOP - 1) {
      s[i] = c;
    }
  }

  if (c == EOF) {
    return EOF;
  }

  if (c == '\n') {
    s[i] = c;
    i++;
  }

  s[i] = '\0';
  return i;
}