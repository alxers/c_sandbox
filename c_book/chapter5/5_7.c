#include <stdio.h>
#include <string.h>

#define MAXLINES 5000

char *lineptr[MAXLINES];

int readlines(char *lineptr[], int nlines);
void writelines(char *lineptr[], int nlines);

void qsort(char *lineptr[], int left, int right);

int main() {
  int nlines;

  char arr[1000];

  if ((nlines = readlines(lineptr, MAXLINES, arr)) >= 0) {
    qsort(lineptr, 0, nlines-1);
    writelines(lineptr, nlines);
    return 0;
  } else {
    printf("error: input too big to sort\n");
    return 1;
  }
}


#define MAXLEN 1000

int getline(char *, int);
char *alloc(int);

int readlines(char *lineptr[], int maxlines, char *arr) {
  int len, nlines;
  char *p, line[MAXLEN];

  nlines = 0;

  int arr_lines = 0;

  while ((len = getline(line, MAXLEN)) > 0) {
    // if (nlines >= maxlines || (p = alloc(len)) == NULL) {
    if (nlines >= maxlines || arr_lines >= 1000) {
      return -1;
    } else {
      line[len-1] = '\0';
      strcpy(arr, line);
      lineptr[nlines++] = p;
    }
  }

  return nlines;
}

// void writelines(char *lineptr[], int nlines) {
//   int i;

//   for (i = 0; i < nlines; i++) {
//     printf("%s\n", lineptr[i]);
//   }
// }

void writelines(char *lineptr[], int nlines) {
  while (nlines-- > 0) {
    printf("%s\n", *lineptr++);
  }
}

void qsort(char *v[], int left, int right) {
  int i, last;
  void swap(char *v[], int i, int j);

  if (left >= right) {
    return;
  }

  swap(v, left, (left+right)/2);
  last = left;

  for (i = left+1; i <= right; i++) {
    if (strcmp(v[i], v[left]) < 0) {
      swap(v, ++last, i);
    }
  }

  swap(v, left, last);
  qsort(v, left, last-1);
  qsort(v, last+1, right);
}

void swap(char *v[], int i, int j) {
  char *temp;

  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

// int getline(char *s, int lim) {
//   int c, i;

//   for (i = 0; i < lim - 1 && (c = getchar()) != EOF; ++i, ++s) {
//     *s = c;
//   }

//   if (c == '\n') {
//     *s = c;
//     s++;
//     ++i;
//   }

//   *s = '\0';
//   return i;
// }

int getline(char s[], int lim) {
  int c, i, m;

  for (i = 0; (c = getchar()) != EOF && c != '\n'; ++i) {
    // Continue iterating over i, but only fill in an array
    // if it's less than global line limit
    // Thus "i" will contain the amount of characters from input (with no limits)
    if (i < lim - 1) {
      m = i;
      s[m] = c;
    }
  }

  if (c == '\n') {
    s[m] = c;
    ++i;
  }

  s[m] = '\0';
  return i;
}


#define ALLOCSIZE 10000

static char allocbuf[ALLOCSIZE];
static char *allocp = allocbuf;

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