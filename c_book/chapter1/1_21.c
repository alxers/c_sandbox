#include <stdio.h>

#define MAXLINE 100
#define DEFAULT_TAB_LENGTH 3

int getline(char s[], int lim) {
  int c, i;
  int space_count = 0;
  int non_space_count = 0;
  int last_valid_char_position = -1;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    // printf("%c\n", c);
    if (c == ' ') {
      space_count++;
      if (space_count % DEFAULT_TAB_LENGTH == 0) {
        s[last_valid_char_position+1] = '\t';
        last_valid_char_position++;
        space_count = 0;
      }
    } else {
      if (space_count) {
        for (int j = 0; j < space_count; j++) {
          s[last_valid_char_position+1] = ' ';
          last_valid_char_position++;
        }
        if (c != ' ') {
          s[last_valid_char_position+1] = c;
          last_valid_char_position++;
        }
        space_count = 0;
      } else {
        s[last_valid_char_position+1] = c;
        last_valid_char_position++;
      }
    }
  }

  if (c == '\n' && last_valid_char_position != 0) {
    s[last_valid_char_position+1] = c;
    ++last_valid_char_position;
  }

  s[last_valid_char_position+1] = '\0';
  printf("%s\n", s);
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

  int max = 0;

  while ((len = getline(line, MAXLINE)) > 0) {
  }

  return 0;
}