#include <stdio.h>

#define MAXLINE 10

void reverse(char s[]) {
  char reversed[MAXLINE];

  for (int i = 0; i < MAXLINE; i++) {
    reversed[i] = 0;
  }

  int curr = 0;
  for (int i = MAXLINE - 1; i >= 0; i--) {
    if (s[i] != '\0') {
      reversed[curr++] = s[i];
    }
  }

  int j;
  for (j = 0; j < curr; j++) {
    if (s[j] != '\0') {
      s[j] = reversed[j];
    }
  }
  s[j] = '\0';
}

int getline(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    s[i] = c;
  }

  if (c == '\n' && i != 0) {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
  // print reversed
  reverse(s);
  for (int k = 0; k < lim - 1; k++) {
    printf("%c", s[k]);
  }
  printf("\n");
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
  for (int i = 0; i < MAXLINE; i++) {
    line[i] = 0;
  }
  char longest[MAXLINE];

  int max = 0;

  while ((len = getline(line, MAXLINE)) > 0) {
    if (len > max) {
      max = len;
      copy(longest, line);
    }
    for (int i = 0; i < MAXLINE; i++) {
      line[i] = 0;
    }
  }

  printf("len: %d, line: %s", max, longest);

  return 0;
}