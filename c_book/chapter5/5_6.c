#include <stdio.h>
#include <stdlib.h>


#define MAXLINE 1000

int old_getline(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    s[i] = c;
  }

  if (c == '\n') {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
  return i;
}

int getline(char *s, int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i, ++s) {
    *s = c;
  }

  if (c == '\n') {
    *s = c;
    s++;
    ++i;
  }

  *s = '\0';
  return i;
}

int old_atoi(char s[]) {
  int i, n, sign;
  for (i = 0; isspace(s[i]); i++) /* skip white space */
    ;
  sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-') /* skip sign */
    i++;
  for (n = 0; isdigit(s[i]); i++)
    n = 10 * n + (s[i] - '0');
  return sign * n;
}

int atoi2(char *s) {
  int n, sign;
  for ( ; isspace(*s); s++) /* skip white space */
    ;
  sign = (*s == '-') ? -1 : 1;
  if (*s == '+' || *s == '-') /* skip sign */
    s++;
  for (n = 0; isdigit(*s); s++)
    n = 10 * n + (*s - '0');
  return sign * n;
}

void old_reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void old_itoa(int n, char s[]) {
  int i, sign;

  if ((sign = n) < 0) /* record sign */
    n = -n; /* make n positive */
  
  i = 0;

  do { /* generate digits in reverse order */
    s[i++] = n % 10 + '0'; /* get next digit */
  } while ((n /= 10) > 0); /* delete it */
    if (sign < 0)
      s[i++] = '-';
  
  s[i] = '\0';
  old_reverse(s);
}

void reverse(char *s) {
  int c, i, j;

  // for (i = 0, j = strlen(s) - 1; i != j; s++, i++, j--) {
  //   c = *s;
  //   *s = *(s + j - i);
  //   *(s + j - i) = c;
  // }

  i = 0;
  j = strlen(s) - 1;
  while (1) {
    c = *s;
    *s = *(s + j - i);
    *(s + j - i) = c;

    s++;
    i++;
    j--;

    if ((i-j) == 1 || i == j) {
      break;
    }
  }
}

void itoa2(int n, char *s) {
  int i, sign;

  if ((sign = n) < 0) /* record sign */
    n = -n; /* make n positive */

  i = 0;
  do { /* generate digits in reverse order */
    *s++ = n % 10 + '0'; /* get next digit */
    i++;
  } while ((n /= 10) > 0); /* delete it */
    if (sign < 0) {
      *s++ = '-';
      i++;
    }
  
  *s = '\0';
  reverse(s-i);
}

int old_strindex(char s[], char t[]) {
  int s_len = 0;
  int t_len = 0;
  int counter = 0;

  for (int l = 0; s[l] != '\0'; l++) {
    s_len++;
  }

  for (int l = 0; t[l] != '\0'; l++) {
    t_len++;
  }

  for (int i = s_len-1; i >= 0; i--) {
    for (int j = t_len-1; j >= 0; j--) {
      if (s[i] == t[j]) {
        counter++;
        break;
      }
    }

    if (counter == t_len) {
      return i;
    }
  }

  return -1;
}

// int strindex(char *s, char *t) {
//   int index = 0;
//   int counter = 0;
//   // int s_len = strlen(s);
//   // int t_len = strlen(t);

//   while (*s != '\0') {
//     if (*t == '\0') {
//       return index;
//     }

//     if (*s == *t) {
//       index = counter;
//       counter++;
//       s++;
//       t++;
//       continue;
//     }

//     counter++;
//   }

//   if (!index) {
//     return -1;
//   }
//   return index;
// }

// int strindex(char *s, char *t) {
//   int s_len = strlen(s);
//   int t_len = strlen(t);
//   int counter = 0;
//   int t_counter = 0;
//   int index = -1;

//   char *s_start = s;
//   s = s + (s_len - 1);
//   t = t + (t_len - 1);

//   int is_match = 0;

//   while (counter < s_len) {
//     if (t_counter > t_len) {
//       return -1;
//     }
//     if (t_counter == t_len && is_match) {
//       return index;
//     }

//     s--;
//     if (*t == *s) {
//       t--;
//       t_counter++;
//       index = counter;
//       is_match = 1;
//     } else {
//       is_match = 0;
//     }

//     counter++;
//   }

//   return index;
// }

int strindex(char *s, char *t) {
  int s_len = strlen(s);
  int t_len = strlen(t);
  int index = s_len;

  char *s_start = s;

  s = s + (s_len - 1);
  t = t + (t_len - 1);

  while (*s != *t) {
    if (s == s_start) {
      return -1;
    }
    s--;
    index--;
  }

  int counter = 1;
  while (*s == *t) {
    if (counter == t_len) {
      return index-1;
    }
    s--;
    t--;
    index--;
    counter++;
  }

  return -1;
}

// int main() {

  // getline
  // int len;
  // char line[MAXLINE];

  // int max = 0;

  // while ((len = getline(line, MAXLINE)) > 0) {
  //   if (len > 5) {
  //     printf("len: %d, line: %s", len, line);
  //   }
  // }
  // end getline

  // atoi
  // printf("old atoi 123 is: %d\n", old_atoi("123"));
  // printf("atoi 123 is: %d\n", atoi2("123"));
  // end atoi

  // itoa
  // char a[4];
  // char b[4];
  // char c[4];
  // old_itoa(-123, a);
  // printf("old itoa 123 is: %s\n", a);
  // itoa2(123, b);
  // printf("itoa 123 is: %s\n", b);
  // itoa2(-123, b);
  // printf("itoa -123 is: %s\n", b);
  // end itoa

  // strindex
  // char s[] = "Haaaelaaaelo";
  // char t[] = "ael";

  // char s2[] = "aelHaaaeaaaeo";
  // char s3[] = "Haaaelaaael";
  // // char s[] = "el";
  // // char t[] = "el";
  // printf("%d\n", old_strindex(s, t));
  // printf("%d\n", strindex(s, t));
  // printf("%d\n", strindex(s2, t));
  // printf("%d\n", strindex(s3, t));
  // end strindex

//   return 0;
// }


// Last part, getop

#define MAXOP 100
#define NUMBER '0'

int getop(char *s);
void push(double);
double pop(void);

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

#include <ctype.h>

int getch(void);
void ungetch(int);

int old_getop(char s[]) {
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

int getop(char *s) {
  int i, c;

  while ((*s = c = getch()) == ' ' || c == '\t') {

  }

  int next_c;

  if (c == '-') {
    next_c = getch();
    if (isdigit(next_c)) {
      c = next_c;
      *++s = c;
      while (isdigit(*++s = c = getch())) {

      }
    }

    *++s = '\0';

    if (next_c == '\n') {
      is_eof = 1;
    }
  } else {
    *++s = '\0';
  }

  if (isdigit(c)) {
    while (isdigit(*++s = c = getch())) {

    }
  }

  if (c == '.') {
    while (isdigit(*++s = c = getch())) {

    }
  }

  if (!isdigit(c) && c != '.' && c != ' ') {
    return c;
  }

  *s = '\0';
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

