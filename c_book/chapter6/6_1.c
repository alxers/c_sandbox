/* 6.1 original code

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct key {
  char *word;
  int count;
} keytab[] = {
  "auto", 0,
  "break", 0,
  "case", 0,
  "char", 0,
  "const", 0,
  "continue", 0,
  "default", 0,
  "int", 0,
  "unsigned", 0,
  "void", 0,
  "volatile", 0,
  "while", 0
};

#define MAXWORD 100
#define NKEYS (sizeof(keytab) / sizeof(keytab[0]))

int getword(char *, int);
int binsearch(char *, struct key *, int);

int main() {
  int n;
  char word[MAXWORD];

  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      if ((n = binsearch(word, keytab, NKEYS)) >= 0) {
        keytab[n].count++;
      }
    }
  }

  for (n = 0; n < NKEYS; n++) {
    if (keytab[n].count > 0) {
      printf("%4d %s\n",
      keytab[n].count, keytab[n].word);
    }
  }

  return 0;
}

int binsearch(char *word, struct key tab[], int n) {
  int cond;
  int low, high, mid;

  low = 0;
  high = n - 1;

  while (low <= high) {
    mid = (low + high) / 2;
    if ((cond = strcmp(word, tab[mid].word)) < 0) {
      high = mid - 1;
    } else if (cond > 0) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
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

// 6.1 with pointers original code

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct key {
  char *word;
  int count;
} keytab[] = {
  "auto", 0,
  "break", 0,
  "case", 0,
  "char", 0,
  "const", 0,
  "continue", 0,
  "default", 0,
  "int", 0,
  "unsigned", 0,
  "void", 0,
  "volatile", 0,
  "while", 0
};

#define MAXWORD 100
#define NKEYS (sizeof(keytab) / sizeof(keytab[0]))

int getword(char *, int);
struct key *binsearch(char *, struct key *, int);

int main() {
  char word[MAXWORD];
  struct key *p;

  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      if ((p = binsearch(word, keytab, NKEYS)) != NULL) {
        p->count++;
      }
    }
  }

  for (p = keytab; p < keytab + NKEYS; p++) {
    if (p->count > 0) {
      printf("%4d %s\n", p->count, p->word);
    }
  }

  return 0;
}

struct key *binsearch(char *word, struct key tab[], int n) {
  int cond;
  struct key *low = &tab[0];
  struct key *high = &tab[n];
  struct key *mid;

  while (low < high) {
    mid = low + (high - low) / 2;
    if ((cond = strcmp(word, mid->word)) < 0) {
      high = mid;
    } else if (cond > 0) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return NULL;
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

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct key {
  char *word;
  int count;
} keytab[] = {
  "\"", 0,
  "/*", 0,
  "//", 0,
  "auto", 0,
  "break", 0,
  "case", 0,
  "char", 0,
  "const", 0,
  "continue", 0,
  "default", 0,
  "int", 0,
  "size_t", 0,
  "unsigned", 0,
  "void", 0,
  "volatile", 0,
  "while", 0
};

#define MAXWORD 100
#define NKEYS (sizeof(keytab) / sizeof(keytab[0]))

int getword(char *, int);
int binsearch(char *, struct key *, int);

int main() {
  int n;
  char word[MAXWORD];

  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      if ((n = binsearch(word, keytab, NKEYS)) >= 0) {
        keytab[n].count++;
      }
    } else if (word[0] == '"') {
      if ((n = binsearch("\"", keytab, NKEYS)) >= 0) {
        keytab[n].count++;
      }
    } else if (word[0] == '/' && word[1] == '/') {
      if ((n = binsearch("//", keytab, NKEYS)) >= 0) {
        keytab[n].count++;
      }
    } else if (word[0] == '/' && word[1] == '*') {
      if ((n = binsearch("/*", keytab, NKEYS)) >= 0) {
        keytab[n].count++;
      }
    }
  }

  for (n = 0; n < NKEYS; n++) {
    if (keytab[n].count > 0) {
      printf("%4d %s\n",
      keytab[n].count, keytab[n].word);
    }
  }

  return 0;
}

int binsearch(char *word, struct key tab[], int n) {
  int cond;
  int low, high, mid;

  low = 0;
  high = n - 1;

  while (low <= high) {
    mid = (low + high) / 2;
    if ((cond = strcmp(word, tab[mid].word)) < 0) {
      high = mid - 1;
    } else if (cond > 0) {
      low = mid + 1;
    } else {
      return mid;
    }
  }

  return -1;
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
  if (!isalpha(c) && c != '_' && c != '/' && c != '*') {
    *w = '\0';
    return c;
  }

  for ( ; --lim > 0; w++) {
    *w = getch();
    if (!isalnum(*w) && *w != '_' && *w != '/' && *w != '*') {
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

//test "test" test size_t int
^Z
   2 "
   1 //
   1 int
   1 size_t

*/