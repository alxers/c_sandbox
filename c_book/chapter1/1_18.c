#include <stdio.h>

#define MAXLINE 10

int getline(char s[], int lim) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    s[i] = c;
  }

  for (int j = i; j > 0; j--) {
    if (s[j] == ' ' || s[j] == '\t' || s[j] == '\a' || s[j] == '\0') {
      s[j] = '\0';
      i--;
    } else {
      i++;
      break;
    }
  }

  if (c == '\n' && i != 0) {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
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

  printf("len: %d, line: %s", max, longest);

  return 0;
}