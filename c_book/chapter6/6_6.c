#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

#define MAXWORD 100
#define MAXARR 10

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
  if (!isalpha(c) && c != '#') {
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

///////////////

struct nlist {
  struct nlist *next;
  char *name;
  char *defn;
};

// #define HASHSIZE 101
#define HASHSIZE 11

static struct nlist *hashtab[HASHSIZE];

unsigned hash(char *s) {
  unsigned hashval;

  for (hashval = 0; *s != '\0'; s++) {
    hashval = *s + 31 *hashval;
  }

  return hashval % HASHSIZE;
}

struct nlist *lookup(char *s) {
  struct nlist *np;

  for (np = hashtab[hash(s)]; np != NULL; np = np->next) {
    if (strcmp(s, np->name) == 0) {
      printf("Lookup %s %s\n", np->name, np->defn);
      return np;
    }
  }

  return NULL;
}

struct nlist *install(char *name, char *defn) {
  struct nlist *np;
  unsigned hashval;

  if ((np = lookup(name)) == NULL) {
    np = (struct nlist *) malloc(sizeof(*np));
    if (np == NULL || (np->name = strdup(name)) == NULL) {
      return NULL;
    }
    hashval = hash(name);
    np->next = hashtab[hashval];
    hashtab[hashval] = np;
  } else {
    free((void *) np->defn);
  }

  if ((np->defn = strdup(defn)) == NULL) {
    return NULL;
  }
  if (np->name) {
    printf("installed %s %s\n", np->name, np->defn);
  }
  return np;
}

void undef(char *name) {
  unsigned hashval;
  hashval = hash(name);

  if (hashtab[hashval] != NULL) {
    printf("undef %s %s\n", hashtab[hashval]->name, hashtab[hashval]->defn);
    hashtab[hashval] = NULL;
  }
}

int get_key_val(char *wrd, int lim) {
  int c, getch(void);
  void ungetch(int);
  char *w = wrd;

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
    *w = getch();
    if (!isalnum(*w)) {
      ungetch(*w);
      break;
    }
  }

  *w = '\0';
  return wrd[0];
}

int main() {
  // install("test_name", "some_value");
  // install("second", "s value");
  // printf("Before undef\n");
  // lookup("test_name");
  // undef("test_name");
  // printf("After undef\n");
  // lookup("test_name");
  // printf("Define the same name again\n");
  // install("test_name", "some_value");

  char word[MAXWORD];
  char all_words[MAXARR][MAXWORD] = {{0}};
  int arr_ind = 0;

  char key[MAXWORD];
  char value[MAXWORD];

  struct nlist *np;

  while (getword(word, MAXWORD) != EOF) {
    if (strcmp("#define", word) == 0) {
      key[0] = get_key_val(key, MAXWORD);
      value[0] = get_key_val(value, MAXWORD);
      if (key && value) {
        install(key, value);
      }
      continue;
    }
    if ((np = lookup(word)) != NULL) {
      strcpy(all_words[arr_ind++], np->defn);
    } else if (isalpha(word[0])) {
      strcpy(all_words[arr_ind++], word);
    }
    word[0] = '\0';
    key[0] = '\0';
    value[0] = '\0';
  }

  for (int i = 0; i < MAXARR; i++) {
    if (*all_words[i]) {
      printf("%s ", all_words[i]);
    }
  }
  printf("\n");

  return 0;
}


/* Test cases:

build\main.exe
test
#define IN one
test2 IN two

*/