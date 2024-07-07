#include <stdio.h>

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

int readlines(char *lineptr[]) {
  int len = 0;
  int nlines = 0;
  char line[MAX_LINE_LEN];
  char *p;

  while ((len = getline(line)) > 0) {
    // "len+1", if "alloc(len)" is used, it always allocates
    // one char left and so '\0' is not coppied
    // the result is strings glued together like aaabbbccc
    if (nlines >= MAXLINES || (p = alloc(len+1)) == NULL) {
      return -1;
    } else {
      strcpy(p, line);
      lineptr[nlines++] = p;
    }
  }

  return nlines;
}

int main(int argc, char *argv[]) {
  int default_n = 5;

  // char *input[] = {
  //   "aaa", "bbbbb", "ccccc", "dddd", "ee", "fffffffff", "ggggg",
  //   "hhhhh", "iii", "ggggg", "kkkk", "ll", "mmmmmmmmm", "nnnnn"
  // };

  if (argc > 1) {
    default_n = atoi(argv[1]);
    if (default_n < 0) {
      default_n = -default_n;
    }
  }

  char *lineptr[MAXLINES];
  int input_len = readlines(lineptr);

  int last_n;
  if (input_len <= default_n) {
    last_n = 0;
  } else {
    last_n = input_len - default_n;
  }
  if (last_n >= 0) {
    for (int i = last_n; i < input_len; i++) {
      printf("%s\n", lineptr[i]);
    }
  }

  printf("argv %d\n", default_n);
  return 0;
}



// Check if strcpy(p, line); from the book's code
// is needed
// Update: if string is created via pointer - char *str = "test";
// then it stays in memory even after "readlines" has been executed
// the "lineptr" will contain valid pointer that points to a created string
// (does this create a memory leak???)

// #include <stdio.h>
// #include <string.h>


// int readlines(char *lineptr[]) {
//   int nlines = 0;
//   // char nlines = 0;
//   char *p;

//   // https://www.codingninjas.com/codestudio/library/difference-between-arrays-and-pointers
//   /*
//     Arrays are allocated at compile time.
//     Pointers are allocated at runtime.

//     The scope has complete control over an array.
//     It will correctly allocate the required memory,
//     and when the variable is no longer in scope,
//     the memory will be automatically released.

//     Pointers can cause a memory leak if we build a local pointer
//     that points to dynamic memory and then forgets to release it.
//   */

//   // This doesn't work in main with printf("%s\n", lineptr[1]);
//   // because the when the variable is no longer in scope, memory is released?
//   // char a[] = "abc";
//   // char d[] = "defg";
//   // char h[] = "hi";

//   // char a[] = {'a', 'b', 'c'};
//   // char d[] = {'d', 'e', 'f', 'g'};
//   // char h[] = {'h', 'i'};

//   // And this works in main with printf("%s\n", lineptr[1]);
//   // even though it's defined within anohter function
//   char *a = "abc";
//   char *d = "defg";
//   char *h = "hi";

//   // char line[5];
//   // strcpy(p, line);

//   // lineptr[nlines++] = p;
//   lineptr[nlines++] = &a[0];
//   lineptr[nlines++] = &d[0];
//   lineptr[nlines++] = &h[0];
//   return nlines;
// }

// int main() {
//   printf("Hello\n");
//   printf("sizeof char %zd\n", sizeof(char));
//   printf("sizeof int %zd\n", sizeof(int));

//   char *lineptr[20];
//   int nlines = 0;
//   // readlines(lineptr);
//   // char *name[] = { "Illegal month", "Jan", "Feb", "Mar" };
//   // printf("%s\n", *name[1]);
//   // printf("%s\n", lineptr[1]);

//   char a[] = "abc";
//   char d[] = "defg";
//   char h[] = "hi";

//   // char a[] = {'a', 'b', 'c'};
//   // char d[] = {'d', 'e', 'f', 'g'};
//   // char h[] = {'h', 'i'};

//   // char *a = "abc";
//   // char *d = "defg";
//   // char *h = "hi";

//   // char line[5];
//   // strcpy(p, line);

//   // lineptr[nlines++] = p;
//   lineptr[nlines++] = a;
//   lineptr[nlines++] = d;
//   lineptr[nlines++] = h;

//   printf("%s\n", lineptr[1]);
// }