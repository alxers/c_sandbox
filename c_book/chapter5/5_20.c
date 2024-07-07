
// #include <stdio.h>
// #include <string.h>
// #include <ctype.h>

// #define MAXTOKEN 100

// enum { NAME, PARENS, BRACKETS };

// void dcl(void);
// void dirdcl(void);

// int gettoken(void);
// int tokentype;
// char token[MAXTOKEN];
// char name[MAXTOKEN];
// char datatype[MAXTOKEN];
// char out[1000];

// // main dcl

// int main() {
//   while (gettoken() !=EOF) {
//     strcpy(datatype, token);
//     out[0] = '\0';
//     dcl();
//     if (tokentype != '\n') {
//       printf("syntax error\n");
//     }

//     printf("%s: %s %s\n", name, out, datatype);
//   }

//   return 0;
// }

// // end main dcl

// // main undcl

// // int STAR_DELAY = 0;

// // int main() {
// //   int type;
// //   char temp[MAXTOKEN];

// //   while (gettoken() != EOF) {
// //     strcpy(out, token);
// //     while ((type = gettoken()) != '\n') {
// //       if (STAR_DELAY) {
// //         if (type != PARENS && type != BRACKETS) {
// //           sprintf(temp, "*%s", out);
// //           strcpy(out, temp);
// //         } else {
// //           sprintf(temp, "(*%s)", out);
// //           strcpy(out, temp);
// //         }
// //         STAR_DELAY = 0;
// //       }
// //       if (type == PARENS || type == BRACKETS) {
// //         strcat(out, token);
// //       } else if (type == '*') {
// //         STAR_DELAY = 1;
// //         // sprintf(temp, "(*%s)", out);
// //         // strcpy(out, temp);
// //       } else if (type == NAME) {
// //         sprintf(temp, "%s %s", token, out);
// //         strcpy(out, temp);
// //       } else {
// //         printf("invalid input at %s\n", token);
// //       }
// //     }
// //     printf("%s\n", out);
// //   }
// //   return 0;
// // }

// // end main undcl

// void dcl() {
//   int ns;

//   for (ns = 0; gettoken() == '*'; ) {
//     ns++;
//   }
//   dirdcl();

//   while (ns-- > 0) {
//     strcat(out, " pointer to");
//   }
// }

// void dirdcl() {
//   int type;

//   if (tokentype == '(') {
//     dcl();
//     if (tokentype != ')') {
//       printf("error: missing )\n");
//     }
//   } else if (tokentype == NAME) {
//     strcpy(name, token);
//   } else {
//     printf("error: expected name or (dcl)\n");
//   }

//   while ((type = gettoken()) == PARENS || type == BRACKETS) {
//     if (type == PARENS) {
//       strcat(out, " function returning");
//     } else {
//       strcat(out, " array");
//       strcat(out, token);
//       strcat(out, " of");
//     }
//   }
// }

// int gettoken() {
//   int c, getch();
//   void ungetch();
//   char *p = token;

//   while ((c = getch()) == ' ' || c == '\t') {
//   }

//   if (c == '(') {
//     while (1) {
//       c = getch();
//       if (c == ' ') {
//         continue;
//       } else {
//         break;
//       }
//     }
//     if (c == ')') {
//       strcpy(token, "()");
//       return tokentype = PARENS;
//     } else {
//       ungetch(c);
//       return tokentype = '(';
//     }
//   } else if (c == '[') {
//     *p++ = c;
//     while (1) {
//       int ch = getch();
//       if (ch != ' ') {
//         *p++ = ch;
//       }
//       if (ch == ']') {
//         *p++ = '\0';
//         break;
//       }
//     }
//     return tokentype = BRACKETS;
//   } else if (isalpha(c)) {
//     for (*p++ = c; isalnum(c = getch()); ) {
//       *p++ = c;
//     }
//     *p = '\0';
//     ungetch(c);
//     return tokentype = NAME;
//   } else {
//     return tokentype = c;
//   }
// }

// #define BUFSIZE 100


// char buf[BUFSIZE];
// int bufp = 0;

// int getch(void) {
//   return (bufp > 0) ? buf[--bufp] : getchar();
// }

// void ungetch(int c) {
//   if (bufp >= BUFSIZE) {
//     printf("ungetch: too many characters\n");
//   } else {
//     buf[bufp++] = c;
//   }
// }















#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS, ARG };

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];
int arg_flag = 0;

// main dcl

int main() {
  while (gettoken() != EOF) {
    strcpy(datatype, token);
    out[0] = '\0';
    dcl();
    if (tokentype != '\n') {
      printf("syntax error\n");
    }

    printf("%s: %s %s\n", name, out, datatype);
  }

  return 0;
}

// end main dcl

// main undcl

// int STAR_DELAY = 0;

// int main() {
//   int type;
//   char temp[MAXTOKEN];

//   while (gettoken() != EOF) {
//     strcpy(out, token);
//     while ((type = gettoken()) != '\n') {
//       if (STAR_DELAY) {
//         if (type != PARENS && type != BRACKETS) {
//           sprintf(temp, "*%s", out);
//           strcpy(out, temp);
//         } else {
//           sprintf(temp, "(*%s)", out);
//           strcpy(out, temp);
//         }
//         STAR_DELAY = 0;
//       }
//       if (type == PARENS || type == BRACKETS) {
//         strcat(out, token);
//       } else if (type == '*') {
//         STAR_DELAY = 1;
//       } else if (type == NAME) {
//         sprintf(temp, "%s %s", token, out);
//         strcpy(out, temp);
//       } else {
//         printf("invalid input at %s\n", token);
//       }
//     }
//     printf("%s\n", out);
//   }
//   return 0;
// }

// end main undcl

void dcl() {
  int ns;

  for (ns = 0; gettoken() == '*'; ) {
    ns++;
  }
  dirdcl();

  while (ns-- > 0) {
    strcat(out, " pointer to");
  }
}

void dirdcl() {
  int type;

  if (tokentype == '(') {
    dcl();
    if (tokentype != ')') {
      printf("error: missing )\n");
    }
  // In case of "void *comp(int)" it doesn't reach this line
  // "gettoken()" is called from while loop below.
  // it returns ARG but it's never recorded after
  // } else if (tokentype == ARG) {
  //   strcpy(arg, token);
  } else if (tokentype == NAME) {
    strcpy(name, token);
  } else {
    printf("error: expected name or (dcl)\n");
  }

  while ((type = gettoken()) == PARENS || type == BRACKETS || type == ARG || arg_flag) {
    if (type == ARG) {
      arg_flag = 1;
      strcat(out, " <with argument ");
      strcat(out, token);
      strcat(out, "> ");
    }
    // 41 is '('
    if (type == PARENS || (arg_flag && type == 41)) {
      arg_flag = 0;
      strcat(out, " function returning");
    } else if (type != ARG) {
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
  }
}

int gettoken() {
  int c, getch();
  void ungetch();
  for (int z = 0; z < MAXTOKEN; z++) {
    token[z] = 0;
  }
  char *p = token;
  int at_least_one_char = 0;

  while ((c = getch()) == ' ' || c == '\t') {
  }

  if (c == '(') {
    while (1) {
      c = getch();
      if (c == ' ') {
        continue;
      } else if (c == ')') {
        break;
      } else {
        if (isalpha(c)) {
          at_least_one_char = 1;
        }
        if (at_least_one_char && (isalpha(c) || c == ' ')) {
          for (*p++ = c; c = getch(); ) {
            if (isalpha(c) || c == ' ') {
              *p++ = c;
            } else {
              break;
            }
          }
          *p = '\0';
          ungetch(c);
          at_least_one_char = 0;
          return tokentype = ARG;
        } else {
          break; 
        }

        // dcl();

        // return tokentype = ARG;
      }
    }
    if (c == ')') {
      strcpy(token, "()");
      return tokentype = PARENS;
    } else {
      ungetch(c);
      return tokentype = '(';
    }
  } else if (c == '[') {
    *p++ = c;
    while (1) {
      int ch = getch();
      if (ch != ' ') {
        *p++ = ch;
      }
      if (ch == ']') {
        *p++ = '\0';
        break;
      }
    }
    return tokentype = BRACKETS;
  } else if (isalpha(c) || c == ' ') {
    for (*p++ = c; c = getch(); ) {
      if (isalnum(c)) {
        *p++ = c;
      } else if (c == ' ') {
        if (strcmp(token, "const") == 0) {
          *p++ = c;
        } else {
          break;
        }
        // break;
      } else {
        break;
      }
    }
    *p = '\0';
    ungetch(c);
    return tokentype = NAME;
  } else {
    return tokentype = c;
  }
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


/*
Test cases:

char     *   *    argv
argv:  pointer to pointer to char

char (*daytab) [    ]
daytab:  pointer to array[] of char

int daytab(    )
daytab:  function returning int



char **argv
argv: pointer to pointer to char

int (*daytab)[13]
daytab: pointer to array[13] of int

int *daytab[13]
daytab: array[13] of pointer to int

void *comp()
comp: function returning pointer to void

void (*comp)()
comp: pointer to function returning void

char (*(*x())[])()
x: function returning pointer to array[] of
pointer to function returning char

char (*(*x[3])())[5]
x: array[3] of pointer to function returning
pointer to array[5] of char

//////////////

void *comp(int)
comp: function expecting 1 parameter: unnamed parameter (int) returning pointer to void

char (*(*x())[])(int)
x: obsolescent non-prototype function declaration with unknown parameters returning pointer
to array[] of pointer to function expecting 1 parameter: unnamed parameter (int) returning char

char (*(*x(float))[])(int)
x: function expecting 1 parameter: unnamed parameter (float) returning pointer
to array[] of pointer to function expecting 1 parameter: unnamed parameter (int) returning char

const int *comp()
comp: obsolescent non-prototype function declaration with unknown parameters returning
pointer to const int

char (*(*x[3])(int))[5]
x: array[3] of pointer to function expecting 1 parameter: unnamed parameter (int) returning
pointer to array[5] of char

char (*(*x[3])(const int))[5]
x: array[3] of pointer to function expecting 1 parameter: unnamed parameter (const int)
returning pointer to array[5] of char

//////////////


UNDCL test cases:

x () * [] * () char
char (*(*x())[])()

x () * char
char *x()

*/