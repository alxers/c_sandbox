#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Allocator
#define ALLOCSIZE 10000
char allocbuf[ALLOCSIZE];
char *allocp = allocbuf;

char *alloc(int n) {
  if (allocbuf + ALLOCSIZE - allocp >= n) {
    allocp += n;
    return allocp - n;
  } else {
    return 0;
  }
}

void afree(char *p) {
  if (p >= allocbuf && p < allocbuf + ALLOCSIZE) {
    allocp = p;
  }
}

// end Allocator

#define MAXLINES 5000
#define MAX_LINE_LEN 1000
char *lineptr[MAXLINES];

int getline(char line[]) {
  int c;

  for (int i = 0; i < MAX_LINE_LEN; i++) {
    line[i] = 0;
  }

  int counter = 0;
  while ((c = getchar()) != EOF) {
    if (c == '\n') {
      break;
    } else {
      line[counter++] = c;
    }
  }

  line[counter] = '\0';

  // printf("LINE: %s\n", line);

  return counter;
}

int readlines(char *lineptr[], int maxlines) {
  int len = 0;
  int nlines = 0;
  char line[MAX_LINE_LEN];
  char *p;

  while ((len = getline(line)) > 0) {
    // "len+1", if "alloc(len)" is used, it always allocates
    // one char left and so '\0' is not coppied
    // the result is strings glued together like aaabbbccc
    if (nlines >= maxlines || (p = alloc(len+1)) == NULL) {
      return -1;
    } else {
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
  }

  return nlines;
}

void writelines(char *lineptr[], int nlines, int reverse) {
  if (reverse) {
    while (nlines-- > 0) {
      printf("%s\n", *(lineptr+nlines));
    }
  } else {
    while (nlines-- > 0) {
      printf("%s\n", *lineptr++);
    }
  }
}

// int numcmp(char *, char *);
int numcmp(char *s1, char *s2, int fold, int directory) {
  double v1, v2;
  v1 = atof(s1);
  v2 = atof(s2);

  if (v1 < v2)
    return -1;
  else if (v1 > v2)
    return 1;
  else
    return 0;
}

void to_lowercase(char source[], char dest[]) {
  int counter = 0;
  while(source[counter] != '\0') {
    if (source[counter] >= 65 && source[counter] <= 90) {
      dest[counter] = source[counter]+32;
    } else {
      dest[counter] = source[counter];
    }
    counter++;
  }
  dest[counter] = '\0';
}

int f_strcmp(char *s, char *t, int fold, int directory) {
  if (fold || directory) {
    char tmp_str1[] = "";
    char tmp_str2[] = "";
    to_lowercase(s, tmp_str1);
    to_lowercase(t, tmp_str2);

    int i = 0;
    while (1) {
      if (directory && !fold) {
        // int dir_symbols = (s[i] != 32) || !(s[i] >= 65 && s[i] <= 90) || !(s[i] >= 97 && s[i] <= 122) &&
        //   (t[i] != 32) || !(t[i] >= 65 && t[i] <= 90) || !(t[i] >= 97 && t[i] <= 122);
        int dir_symbols = (
              (s[i] == 32) ||
              (s[i] >= 65 && s[i] <= 90) ||
              (s[i] >= 48 && s[i] <= 57) ||
              (s[i] >= 97 && s[i] <= 122)
            ) && (
              (t[i] == 32) ||
              (t[i] >= 65 && t[i] <= 90) ||
              (t[i] >= 48 && t[i] <= 57) ||
              (t[i] >= 97 && t[i] <= 122)
            );
        if (!dir_symbols) {
          if (s[i] == '\0') {
            return 0;
          } else {
            i++;
            continue;
          }
        }
        if (s[i] == t[i]) {
          if (s[i] == '\0') {
            return 0;
          }
        } else {
          // break;
          return s[i] - t[i];
        }
      } else if (!directory && fold) {
        if (tmp_str1[i] == tmp_str2[i]) {
          if (tmp_str1[i] == '\0') {
            return 0;
          }
        } else {
          // break;
          return tmp_str1[i] - tmp_str2[i];
        }
      } else if (directory && fold) {
        int a_dir_symbols = (
              (tmp_str1[i] == 32) ||
              (tmp_str1[i] >= 65 && tmp_str1[i] <= 90) ||
              (tmp_str1[i] >= 48 && tmp_str1[i] <= 57) ||
              (tmp_str1[i] >= 97 && tmp_str1[i] <= 122)
            ) && (
              (tmp_str2[i] == 32) ||
              (tmp_str2[i] >= 65 && tmp_str2[i] <= 90) ||
              (tmp_str2[i] >= 48 && tmp_str2[i] <= 57) ||
              (tmp_str2[i] >= 97 && tmp_str2[i] <= 122)
            );
        if (!a_dir_symbols) {
          if (tmp_str1[i] == '\0') {
            return 0;
          } else {
            i++;
            continue;
          }
        }
        if (tmp_str1[i] == tmp_str2[i]) {
          if (tmp_str1[i] == '\0') {
            return 0;
          }
        } else {
          // break;
          return tmp_str1[i] - tmp_str2[i];
        }
      }
      i++;
    }


    // for (i = 0; tmp_str1[i] == tmp_str2[i]; i++) {
    //   if (tmp_str1[i] == '\0') {
    //     return 0;
    //   }
    // }
    return tmp_str1[i] - tmp_str2[i];

  } else {
    for ( ; *s == *t; s++, t++) {
      if (*s == '\0') {
        return 0;
      }
    }
    return *s - *t;
  }
}

void swap(void *v[], int i, int j) {
  void *temp;
  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void qsort(
    void *v[], int left, int right, int (*comp)(void *, void *, int, int),
    int fold, int directory
  ) {
  int i, last;
  void swap(void *v[], int, int);
  if (left >= right) /* do nothing if array contains */
    return; /* fewer than two elements */

  swap(v, left, (left + right)/2);
  last = left;
  for (i = left+1; i <= right; i++) {
    if ((*comp)(v[i], v[left], fold, directory) < 0) {
      swap(v, ++last, i);
    }
  }

  swap(v, left, last);
  qsort(v, left, last-1, comp, fold, directory);
  qsort(v, last+1, right, comp, fold, directory);
}

int main(int argc, char *argv[]) {
  int nlines; /* number of input lines read */

  int numeric = 0; /* 1 if numeric sort */
  int reverse = 0; /* 1 if reverse order sort */
  int fold = 0;    /* 1 if fold upper and lower case */
  int directory = 0;

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-n") == 0) {
        numeric = 1;
      } else if (strcmp(argv[i], "-r") == 0) {
        reverse = 1;
      } else if (strcmp(argv[i], "-f") == 0) {
        fold = 1;
      } else if (strcmp(argv[i], "-d") == 0) {
        directory = 1;
      }
    }
  }

  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    qsort(
      (void**) lineptr, 0, nlines-1, (int (*)(void*,void*,int,int))(numeric ? numcmp : f_strcmp),
      fold, directory
    );
    writelines(lineptr, nlines, reverse);
    return 0;
  } else {
    printf("input too big to sort \n");
    return 1;
  }
}

/*
test cases:

build\main.exe
abcde
abcdef
abc
a
^Z
a
abc
abcde
abcdef

build\main.exe
aaa
ccc
bbb
ddd
^Z
aaa
bbb
ccc
ddd

build\main.exe -n
20
10
5
^Z
5
10
20

build\main.exe -n
bbb
aaa
ccc
^Z
aaa
bbb
ccc

build\main.exe -f
a
A

a
A

build\main.exe
%
!

!
%

build\main.exe -d
%
!

%
!

*/