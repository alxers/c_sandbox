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

int global_num = 4;

struct tnode *addtree(struct tnode *, char *);
void treeprint(struct tnode *);
int getword(char *, int);

int main(int argc, char **argv) {
  struct tnode *root;
  char word[MAXWORD];

  if (argv[1]) {
    global_num = atoi(argv[1]);
  }

  root = NULL;
  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      root = addtree(root, word);
    }
    word[0] = '\0';
  }

  treeprint(root);
  return 0;
}

void cpy_str(char *d, char *s) {
  for (int i = 0; i < global_num; i++) {
    if (*s != '\0') {
      *d = *s;
      d++;
      s++;
    }
  }
  *d = '\0';
}

struct tnode *talloc(void);
char *strdup(char *);

struct tnode *addtree(struct tnode *p, char *w) {
  int cond;

  if (w[0] == '"' || (w[0] == '/' && (w[1] == '/' || w[1] == '*'))) {
    return p;
  }

  char new_w[MAXWORD];
  cpy_str(new_w, w);

  if (p == NULL) {
    p = talloc();
    p->word = strdup(new_w);
    p->count = 1;
    p->left = p->right = NULL;
  } else if ((cond = strcmp(new_w, p->word)) == 0) {
    p->count++;
  } else if (cond < 0) {
    p->left = addtree(p->left, new_w);
  } else {
    p->right = addtree(p->right, new_w);
  }
  return p;
}

void treeprint(struct tnode *p) {
  if (p != NULL) {
    treeprint(p->left);
    if (p->count > 1) {
      printf("%4d %s\n", p->count, p->word);
    }
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

  int next_c;

  while (isspace(c = getch())) {
  }

  if (c == '"') {
    while (!isspace(c = getch())) {
    }
    return 0;
  } else if (c == '/') {
    next_c = getch();
    if (next_c == '/' || next_c == '*') {
      while (next_c != '\n') {
        next_c = getch();
      }
      return 0;
    }
    ungetch(next_c);
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
    if (!isalnum(*w)) {
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

/*

Test cases:

\main.exe
test123
test789
test456
abc123
abc456
while
^Z
   3 test

\main.exe 3
test123
test789
test456
abc123
abc456
while
^Z
   2 abc
   3 tes

\main.exe
test123
test456
test
"test"
//test
// test
/*test
/* test
^Z
   3 test

*/















/*
// 6.2 original code

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
