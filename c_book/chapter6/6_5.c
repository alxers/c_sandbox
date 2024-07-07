#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

int main() {
  install("test_name", "some_value");
  install("second", "s value");
  printf("Before undef\n");
  lookup("test_name");
  undef("test_name");
  printf("After undef\n");
  lookup("test_name");
  printf("Define the same name again\n");
  install("test_name", "some_value");

  return 0;
}