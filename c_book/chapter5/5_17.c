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

// Global variables for command line arguments
int NUMERIC = 0; /* 1 if numeric sort */
int NUMERIC_NUM = 0; /* field number, f.e. we want to sort numerically by the second in: abc 123 def */
int NUMERIC_ORD = 0;
int REVERSE = 0; /* 1 if reverse order sort */
int REVERSE_NUM = 0;
int REVERSE_ORD = 0;
int FOLD = 0;    /* 1 if fold upper and lower case */
int FOLD_NUM = 0;
int FOLD_ORD = 0;
int DIRECTORY = 0;
int DIRECTORY_NUM = 0;
int DIRECTORY_ORD = 0;
int ARGS_ORD[4] = {0};
//

void writelines(char *lineptr[], int nlines) {
  // if (REVERSE) {
  //   while (nlines-- > 0) {
  //     printf("%s\n", *(lineptr+nlines));
  //   }
  // } else {
  //   while (nlines-- > 0) {
  //     printf("%s\n", *lineptr++);
  //   }
  // }
  while (nlines-- > 0) {
    printf("%s\n", *lineptr++);
  }
}

int numcmp(char *s1, char *s2) {
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

int *start_end_ind(char *s, char *t, int num, int arr[]) {
  // Find n-th space for the first and second strings
  // where "n" is argument's _NUM-1
  int first_str_space_count = 0;
  int second_str_space_count = 0;

  int first_str_start_ind = -1;
  int second_str_start_ind = -1;

  int first_str_end_ind = MAX_LINE_LEN;
  int second_str_end_ind = MAX_LINE_LEN;

  int first_done = 0;
  int second_done = 0;

  int ind = 0;
  while (1) {
    if (!first_done && s[ind] == '\0') {
      first_done = 1;
    }
    if (!second_done && t[ind] == '\0') {
      second_done++;
    }

    if (!first_done && s[ind] == 32) {
      first_str_space_count++;

      if (first_str_space_count == num-1) {
        first_str_start_ind = ind;
      }
      if (first_str_space_count == num) {
        first_str_end_ind = ind;
      }
    }
    if (!second_done && t[ind] == 32) {
      second_str_space_count++;

      if (second_str_space_count == num-1) {
        second_str_start_ind = ind;
      }
      if (second_str_space_count == num) {
        second_str_end_ind = ind;
      }
    }

    ind++;

    if (first_done && second_done) {
      break;
    }
  }

  arr[0] = first_str_start_ind;
  arr[1] = first_str_end_ind;
  arr[2] = second_str_start_ind;
  arr[3] = second_str_end_ind;

  return arr;
}

int f_strcmp(char *s, char *t, int f_start, int f_end) {
  int i = 0;
  while (1) {
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

    if (DIRECTORY && !dir_symbols) {
      if (s[i] == '\0') {
        return 0;
      } else {
        i++;
        continue;
      }
    } else {
      if (i >= f_end) {
        return 0;
      }
      if (s[i] == t[i]) {
        if ((s[i] == '\0') || (s[i] == 32)) {
          return 0;
        }
      } else {
        return s[i] - t[i];
      }
    }
    i++;
  }

  return s[i] - t[i];
}

int f_cmp(char *s, char *t) {
  int reverse_action = 1;
  int dir_action = 1;
  int num_action = 1;
  int fold_action = 1;
  int total_action = 0;

  int rev_first_str_start_ind = -1;
  int rev_second_start_ind = -1;
  int rev_first_end_ind = MAX_LINE_LEN;
  int rev_second_end_ind = MAX_LINE_LEN;

  int num_first_str_start_ind = -1;
  int num_second_start_ind = -1;
  int num_first_end_ind = MAX_LINE_LEN;
  int num_second_end_ind = MAX_LINE_LEN;

  int dir_first_str_start_ind = -1;
  int dir_second_start_ind = -1;
  int dir_first_end_ind = MAX_LINE_LEN;
  int dir_second_end_ind = MAX_LINE_LEN;

  int fold_first_str_start_ind = -1;
  int fold_second_start_ind = -1;
  int fold_first_end_ind = MAX_LINE_LEN;
  int fold_second_end_ind = MAX_LINE_LEN;
  char tmp_str1[MAX_LINE_LEN];
  to_lowercase(s, tmp_str1);
  char tmp_str2[MAX_LINE_LEN];
  to_lowercase(t, tmp_str2);


  if (REVERSE && !REVERSE_NUM) {
    reverse_action = -1;
  }

  if (REVERSE_NUM) {
    int rev_arr[4];
    start_end_ind(s, t, REVERSE_NUM, rev_arr);
    rev_first_str_start_ind = rev_arr[0];
    rev_first_end_ind = rev_arr[1];
    rev_second_start_ind = rev_arr[2];
    rev_second_end_ind = rev_arr[3];
  }

  if (NUMERIC) {
    if (NUMERIC_NUM) {
      int num_arr[4];
      start_end_ind(s, t, NUMERIC_NUM, num_arr);
      num_first_str_start_ind = num_arr[0];
      num_first_end_ind = num_arr[1];
      num_second_start_ind = num_arr[2];
      num_second_end_ind = num_arr[3];
    }
  }

  if (DIRECTORY) {
    if (DIRECTORY_NUM) {
      int dir_arr[4];
      start_end_ind(s, t, DIRECTORY_NUM, dir_arr);
      dir_first_str_start_ind = dir_arr[0];
      dir_first_end_ind = dir_arr[1];
      dir_second_start_ind = dir_arr[2];
      dir_second_end_ind = dir_arr[3];
    }
  }

  if (FOLD) {
    if (FOLD_NUM) {
      int fold_arr[4];
      start_end_ind(s, t, FOLD_NUM, fold_arr);
      fold_first_str_start_ind = fold_arr[0];
      fold_first_end_ind = fold_arr[1];
      fold_second_start_ind = fold_arr[2];
      fold_second_end_ind = fold_arr[3];
    }
  }

  if (!NUMERIC && !DIRECTORY && !FOLD) {
    return f_strcmp(s, t, 0, MAX_LINE_LEN) * reverse_action;
  }

  int number_of_args = 4;
  for (int i = number_of_args-1; i >= 0; i--) {
    switch (ARGS_ORD[i]) {
      case 'R':
        // TODO: 
        /*
        determine first start, second start indexes for reverse (if provided)
        Then check if NUMERIC were provided

        If NUMERIC_NUM == REVERSE_NUM => total_action = numcpm(s+reverse_num, t...) * (-1)
        If NUMERIC (gloabal) => total_action = numcpm(s+reverse_num, t...) * (-1)
        IF NUMERIC_NUM != REVERSE_NUM AND !NUMERIC (global) => call strcmp * (-1)
        */
        // reverse_action = -1;
        if (REVERSE_NUM && REVERSE_NUM == NUMERIC_NUM) {
          total_action = numcmp(
              s+rev_first_str_start_ind+1, t+rev_second_start_ind+1
            ) * (-1);
          reverse_action = 1;
        } else if (REVERSE_NUM && (REVERSE_NUM != NUMERIC_NUM)) {
          total_action = f_strcmp(
              s+rev_first_str_start_ind+1, t+rev_second_start_ind+1,
              rev_first_str_start_ind, rev_first_end_ind
          ) * (-1);
          reverse_action = 1;
        }
        break;
      case 'N':
        total_action = numcmp(
            s+num_first_str_start_ind+1, t+num_second_start_ind+1
          );
        break;
      case 'F':
        total_action = f_strcmp(
            tmp_str1+fold_first_str_start_ind+1, tmp_str2+fold_second_start_ind+1,
            fold_first_str_start_ind, fold_first_end_ind
        );
        break;
      case 'D':
        total_action = f_strcmp(
            s+dir_first_str_start_ind+1, t+dir_second_start_ind+1,
            dir_first_str_start_ind, dir_first_end_ind
        );
        break;
    }
  }

  return total_action * reverse_action;
}

void swap(void *v[], int i, int j) {
  void *temp;
  temp = v[i];
  v[i] = v[j];
  v[j] = temp;
}

void qsort(void *v[], int left, int right, int (*comp)(void *, void *)) {
  int i, last;
  void swap(void *v[], int, int);
  if (left >= right) /* do nothing if array contains */
    return; /* fewer than two elements */

  swap(v, left, (left + right)/2);
  last = left;
  for (i = left+1; i <= right; i++) {
    if ((*comp)(v[i], v[left]) < 0) {
      swap(v, ++last, i);
    }
  }

  swap(v, left, last);
  qsort(v, left, last-1, comp);
  qsort(v, last+1, right, comp);
}

int main(int argc, char *argv[]) {
  int nlines; /* number of input lines read */

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-n") == 0) {
        NUMERIC = 1;
        ARGS_ORD[i-1] = 'N';
        if (argc >= i+1 && argv[i+1] && atoi(argv[i+1]) > 0) {
          NUMERIC_NUM = atoi(argv[i+1]);
          // NUMERIC_ORD = i;
        }
      } else if (strcmp(argv[i], "-r") == 0) {
        REVERSE = 1;
        ARGS_ORD[i-1] = 'R';
        if (argc >= i+1 && argv[i+1] && atoi(argv[i+1]) > 0) {
          REVERSE_NUM = atoi(argv[i+1]);
          // REVERSE_ORD = i;
        }
      } else if (strcmp(argv[i], "-f") == 0) {
        FOLD = 1;
        ARGS_ORD[i-1] = 'F';
        if (argc >= i+1 && argv[i+1] && atoi(argv[i+1]) > 0) {
          FOLD_NUM = atoi(argv[i+1]);
          // FOLD_ORD = i;
        }
      } else if (strcmp(argv[i], "-d") == 0) {
        DIRECTORY = 1;
        ARGS_ORD[i-1] = 'D';
        if (argc >= i+1 && argv[i+1] && atoi(argv[i+1]) > 0) {
          DIRECTORY_NUM = atoi(argv[i+1]);
          // DIRECTORY_ORD = i;
        }
      }
    }
  }

  if ((nlines = readlines(lineptr, MAXLINES)) >= 0) {
    // qsort((void**) lineptr, 0, nlines-1, (int (*)(void*,void*))(NUMERIC ? numcmp : f_strcmp));
    qsort((void**) lineptr, 0, nlines-1, (int (*)(void*,void*))(f_cmp));
    writelines(lineptr, nlines);
    return 0;
  } else {
    printf("input too big to sort \n");
    return 1;
  }
}

/*
test cases:

build\main.exe -n 1 -r 2
1 1
2 2
3 3

1 1
2 2
3 3

build\main.exe -r 2 -n 1
1 1
2 2
3 3

3 3
2 2
1 1

build\main.exe -r 2
c a
b b
a c

a c
b b
c a

build\main.exe -d 1 -n 2
%a 456
%a 123
!b 123

%a 123
%a 456
!b 123

build\main.exe -d 2
a b
abcd a h

abcd a h
a b

build\main.exe -d 2
a % a
c ! c

a % a
c ! c

build\main.exe -d 2
a b a
c a c

c a c
a b a

build\main.exe -f -d
%a bc Def
%b aC def

%a bc Def
%b aC def

build\main.exe -n 2
abc 234 def
abc 123 def

abc 123 def
abc 234 def

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