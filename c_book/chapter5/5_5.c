#include <stdio.h>

void strcpy(char *s, char *t, int n) {
  int counter = 0;
  while (counter < n) {
    *s++ = *t++;
    counter++;
  }
}

int old_strcmp(char *s, char *t) {
  for ( ; *s == *t; s++, t++)
    if (*s == '\0')
      return 0;
  return *s - *t;
}

int strcmp(char *s, char *t, int n) {
  int counter = 0;
  for ( ; *s == *t; s++, t++) {
    if (counter == (n - 1)) {
      return *s - *t;
    }
    counter++;
    if (*s == '\0') {
      return 0;
    }
  }

  return *s - *t;
}

void strcat(char *s, char *t, int n) {
  int counter = 0;
  while (*s != '\0') {
    s++;
  }

  while ((counter < n) && (*s++ = *t++) != '\0') {
    counter++;
  }
}

int main() {

  // strcpy
  char a[] = "Hello";
  char b[] = "abc";
  strcpy(a, b, 2);
  printf("strcpy: %s\n\n", a);

  // strcmp
  char c[] = "abcdef";
  char d[] = "abc";
  char e[] = "abmno";
  char f[] = "zzzzzf";
  printf("strcmp: abcdef and abc, first 3: %d\n", strcmp(c, d, 3)); // should be eq
  printf("strcmp: abcdef and abc, first 5: %d\n", strcmp(c, d, 5)); // should be pos
  printf("strcmp: abcdef and abmno, first 2: %d\n", strcmp(c, e, 2)); // should be eq
  printf("strcmp: abcdef and abmno, first 3: %d\n", strcmp(c, e, 3)); // should be negative
  printf("strcmp: abcdef and zzzzzf, first 5: %d\n", strcmp(c, e, 5)); // should be negative

  printf("old_strcmp: abcdef and abc: %d\n", old_strcmp(c, d)); // should be pos
  printf("old_strcmp: abcdef and abmno: %d\n", old_strcmp(c, e)); // should be neg
  printf("old_strcmp: abcdef and abcdef: %d\n\n", old_strcmp(c, c)); // should be eq

  // strcat
  char g[6] = "He";
  char h[4] = "llo";
  strcat(g, h, 3);
  printf("strcat: He and llo, 3: %s\n", g);

  char i[6] = "He";
  char j[4] = "llo";
  strcat(i, j, 1);
  printf("strcat: He and llo, 1: %s\n\n", i);


  return 0;
}