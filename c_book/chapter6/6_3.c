#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define MAXARR 10
// #define MAXARR 1000

int GLOBAL_LINE_NUM = 1;
int IS_NEW_LINE = 0;

struct tnode {
  char *word;
  int line_num[MAXARR];
  struct tnode *left;
  struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

int main() {
  struct tnode *root;
  char word[MAXWORD];

  root = NULL;
  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      root = addtree(root, word);
    }
  }

  treeprint(root);
  return 0;
}

struct tnode *talloc(void);
char *strdup(char *);

struct tnode *addtree(struct tnode *p, char *w) {
  int cond;

  int last_arr_value_ind = MAXARR - 1;

  if (p == NULL) {
    p = talloc();
    for (int i = 0; i < MAXARR; i++) {
      p->line_num[i] = 0;
    }
    p->word = strdup(w);
    p->line_num[0] = GLOBAL_LINE_NUM;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w, p->word)) == 0) {
    for (int i = 0; i < MAXARR; i++) {
      if (p->line_num[i]) {
        last_arr_value_ind = i+1;
      }
    }
    if (GLOBAL_LINE_NUM != last_arr_value_ind) {
      p->line_num[last_arr_value_ind] = GLOBAL_LINE_NUM;
    }
  } else if (cond < 0) {
    p->left = addtree(p->left, w);
  } else {
    p->right = addtree(p->right, w);
  }

  if (IS_NEW_LINE) {
    GLOBAL_LINE_NUM++;
    IS_NEW_LINE = 0;
  }
  return p;
}

void print_arr(int *arr) {
  int arr_size = sizeof(arr) / sizeof(arr[0]);
  for (int i = 0; i <= arr_size; i++) {
    if (arr[i]) {
      printf("%d ", arr[i]);
    }
  }
  printf("\n\n");
}

void treeprint(struct tnode *p) {
  if (p != NULL) {
    treeprint(p->left);

    printf("%s\n", p->word);
    print_arr(p->line_num);

    treeprint(p->right);
  }
}

#include <stdlib.h>

struct tnode *talloc(void) {
  return (struct tnode *) malloc(sizeof(struct tnode));
}

char *strdup(char *s) {
  char *p;

  p = (char *) malloc(strlen(s) + 1); // +1 for '\0'
  if (p != NULL) {
    strcpy(p, s);
  }

  return p;
}

int is_skip_char(char c) {
  char sym[] = {',', '.', '!', '/', '%', '^', '&', '*', '@', '#', '$', '\"', '\'', '\0' };
  int sym_len = sizeof(sym) / sizeof(sym[0]);
  for(int i = 0; i < sym_len; i++) {
    if(sym[i] == c) {
      return 1;
    }
  }

  return 0;
}

int getword(char *word, int lim) {
  int c, getch(void);
  void ungetch(int);
  char *w = word;

  // while (isspace(c = getch())) {
  // }

  // while (is_skip_char(c = getch())) {
  // }

  while(1) {
    c = getch();
    if(isspace(c) || is_skip_char(c)) {
      continue;
    } else {
      break;
    }
  }

  if (c == '\n') {
    IS_NEW_LINE = 1;
  }

  // debug
  if (c == '\n' || *w =='\n') {
    IS_NEW_LINE = 1;
  }

  if (c != EOF) {
    *w++ = c;
  }
  if (!isalpha(c)) {
    *w = '\0';
    return c;
  }

  for ( ; --lim > 0; w++) {
    *w = getch();
    if(is_skip_char(*w)) {
      *w = getch();
    }
    if (*w == '\n') {
      IS_NEW_LINE = 1;
    }
    if (!isalnum(*w)) {
      ungetch(*w);
      break;
    }
  }

  *w = '\0';

  if (strcmp(word, "and") == 0 || strcmp(word, "the") == 0) {
    for (int i = 0; i < MAXWORD; i++) {
      word[i] = 0;
    }
    if (IS_NEW_LINE) {
      GLOBAL_LINE_NUM++;
      IS_NEW_LINE = 0;
    }
    word[0] = '\0';
    return 0;
  }

  return word[0];
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

/* Test cases

----------build\main.exe
abc
def
abc
def
^Z
abc
1 3

def
2 4

-----------build\main.exe
abc def abc
de
def
abc
^Z
abc
1 4

de
2

def
1 3


----------build\main.exe
the abc and abc
the
and
abc
def
^Z
abc
1 4

def
5


------------

build\main.exe
abc
the
abc
^Z
abc
1 3

-----------

Write a cross-referencer that prints a list
of all words in a document,
and for
each word, a list of the line numbers on which it occurs.
Remove noise words like the,
and, and so on


------------------ build\main.exe
aaa, aaa test
test,
and for
aaa, a list
list aaa.
^Z
a
4

aaa
1 4 5

for
3

list
4 5

test
1 2


----------------

aaa, aaa test
test,
and for
aaa, a list
list aaa.


aaa, aaa test
test,
for
aaa, a list
list aaa.


aaa, aaa test
test
for
aaa a list
list aaa.

-------------build\main.exe
aaa,
aaa
^Z
aaa
1 2

same

build\main.exe
aaa,
bbb
^Z
aaa
1

bbb
2

*/

















/*
// 6.1 original code

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100

struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

int main() {
  struct tnode *root;
  char word[MAXWORD];

  root = NULL;
  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      root = addtree(root, word);
    }
  }

  treeprint(root);
  return 0;
}

struct tnode *talloc(void);
char *strdup(char *);

struct tnode *addtree(struct tnode *p, char *w) {
  int cond;

  if (p == NULL) {
    p = talloc();
    p->word = strdup(w);
    p->count = 1;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(w, p->word)) == 0) {
    p->count++;
  } else if (cond < 0) {
    p->left = addtree(p->left, w);
  } else {
    p->right = addtree(p->right, w);
  }
  return p;
}

void treeprint(struct tnode *p) {
  if (p != NULL) {
    treeprint(p->left);
    printf("%4d %s\n", p->count, p->word);
    treeprint(p->right);
  }
}

#include <stdlib.h>

struct tnode *talloc(void) {
  return (struct tnode *) malloc(sizeof(struct tnode));
}

char *strdup(char *s) {
  char *p;

  p = (char *) malloc(strlen(s) + 1); // +1 for '\0'
  if (p != NULL) {
    strcpy(p, s);
  }

  return p;
}

int getword(char *word, int lim) {
  int c, getch(void);
  void ungetch(int);
  char *w = word;

  while (isspace(c = getch())) {
  }

  if (c != EOF) {
    *w++ = c;
  }
  if (!isalpha(c)) {
    *w = '\0';
    return c;
  }

  for ( ; --lim > 0; w++) {
    if (!isalnum(*w = getch())) {
      ungetch(*w);
      break;
    }
  }

  *w = '\0';
  return word[0];
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
*/
