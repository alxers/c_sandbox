#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXWORD 100
#define MAXARR 10

//remove
struct tnode {
  char *word;
  int count;
  struct tnode *left;
  struct tnode *right;
};

struct word_freq {
  char *word;
  int count;
};

int getword(char *, int);

int numcmp(struct word_freq s1, struct word_freq s2) {
  int v1, v2;
  v1 = s1.count;
  v2 = s2.count;

  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return 0;
}

void swap(struct word_freq v[], int i, int j) {
  struct word_freq temp;
  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void qsort(struct word_freq v[], int left, int right, int (*comp)(struct word_freq, struct word_freq)) {
  int i, last;
  void swap(struct word_freq v[], int, int);
  if (left >= right) /* do nothing if array contains */
    return; /* fewer than two elements */

  swap(v, left, (left + right)/2);
  last = left;
  for (i = left+1; i <= right; i++)
    if (-1 * (*comp)(v[i], v[left]) < 0)
      swap(v, ++last, i);

  swap(v, left, last);
  qsort(v, left, last-1, comp);
  qsort(v, last+1, right, comp);
}

int main(int argc, char **argv) {
  struct word_freq wf[MAXARR];
  for (int i = 0; i < MAXARR; i++) {
    wf[i].count = 0;
    wf[i].word = "";
  }
  int last_wf_arr_ind = 0;
  char word[MAXWORD];

  while (getword(word, MAXWORD) != EOF) {
    if (isalpha(word[0])) {
      int i = 0;
      int is_found = 0;
      for ( ; i < MAXARR; i++) {
        int cond = strcmp(word, wf[i].word);
        if (cond == 0) {
          is_found = 1;
          break;
        }
      }

      if (is_found) {
        // last_wf_arr_ind = i;
        wf[i].count++;
        // last_wf_arr_ind++;
        is_found = 0;
      } else {
        wf[last_wf_arr_ind].count = 1;
        wf[last_wf_arr_ind].word = strdup(word);
        last_wf_arr_ind++;
      }
    }
    word[0] = '\0';
  }

  // qsort((void**) lineptr, 0, nlines-1, (int (*)(void*,void*))(numeric ? numcmp : strcmp));
  qsort(wf, 0, MAXARR-1, numcmp);

  for (int i = 0; i < MAXARR; i++) {
    if (wf[i].count) {
      printf("%d %s\n", wf[i].count, wf[i].word);
    }
  }

  return 0;
}

void cpy_str(char *d, char *s) {
  for (int i = 0; i < MAXWORD; i++) {
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

build\main.exe
aaa
aaa
zzz
mmm
mmm
mmm
mmm
kkk
kkk
^Z
4 mmm
2 aaa
2 kkk
1 zzz

build\main.exe
aaa
bbb
bbb
kkk
^Z
2 bbb
1 aaa
1 kkk


-------------

build\main.exe
kkk
mmm
mmm
aaa
ttt
mmm
bbb
aaa
^Z
3 mmm
2 aaa
1 bbb
1 ttt
1 kkk



------------
build\main.exe
bbb
bbb
bbb
aaa
ccc
aaa
^Z
3 bbb
2 aaa
1 ccc

*/