#include <stdio.h>

#define MAXLINE 10

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

void copy(char to[], char from[]) {
  int i = 0;

  while((to[i] = from[i]) != '\0') {
    ++i;
  }
}

int main() {
  int len;
  char line[MAXLINE];
  char longest[MAXLINE];

  int max = 0;

  while ((len = getline(line, MAXLINE)) > 0) {
    if (len > max) {
      max = len;
      copy(longest, line);
    }
  }

  if (max > 0) {
    printf("len: %d, line: %s", max, longest);
  }

  return 0;
}