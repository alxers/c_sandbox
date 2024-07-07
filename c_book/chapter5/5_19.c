#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKEN 100

enum { NAME, PARENS, BRACKETS };

void dcl(void);
void dirdcl(void);

int gettoken(void);
int tokentype;
char token[MAXTOKEN];
char name[MAXTOKEN];
char datatype[MAXTOKEN];
char out[1000];

// main dcl

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

// end main dcl

// main undcl

int STAR_DELAY = 0;

int main() {
  int type;
  char temp[MAXTOKEN];

  while (gettoken() != EOF) {
    strcpy(out, token);
    while ((type = gettoken()) != '\n') {
      if (STAR_DELAY) {
        if (type != PARENS && type != BRACKETS) {
          sprintf(temp, "*%s", out);
          strcpy(out, temp);
        } else {
          sprintf(temp, "(*%s)", out);
          strcpy(out, temp);
        }
        STAR_DELAY = 0;
      }
      if (type == PARENS || type == BRACKETS) {
        strcat(out, token);
      } else if (type == '*') {
        STAR_DELAY = 1;
        // sprintf(temp, "(*%s)", out);
        // strcpy(out, temp);
      } else if (type == NAME) {
        sprintf(temp, "%s %s", token, out);
        strcpy(out, temp);
      } else {
        printf("invalid input at %s\n", token);
      }
    }
    printf("%s\n", out);
  }
  return 0;
}

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
  } else if (tokentype == NAME) {
    strcpy(name, token);
  } else {
    printf("error: expected name or (dcl)\n");
  }

  while ((type = gettoken()) == PARENS || type == BRACKETS) {
    if (type == PARENS) {
      strcat(out, " function returning");
    } else {
      strcat(out, " array");
      strcat(out, token);
      strcat(out, " of");
    }
  }
}

int gettoken() {
  int c, getch();
  void ungetch();
  char *p = token;

  while ((c = getch()) == ' ' || c == '\t') {
  }

  if (c == '(') {
    while (1) {
      c = getch();
      if (c == ' ') {
        continue;
      } else {
        break;
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
  } else if (isalpha(c)) {
    for (*p++ = c; isalnum(c = getch()); ) {
      *p++ = c;
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
argv: pointer to char

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


UNDCL test cases:

x () * [] * () char
char (*(*x())[])()

x () * char
char *x()

*/