#include <stdio.h>

#define MAXLINE 100
#define INPUT_LINE_LIMIT 5

int getline(char s[], int lim) {
  int c, i;
  int curr_row_char_count = 0;
  int last_blank_char_pos = -1;
  int row = 1;

  int offset = 0;
  int cr = 0;

  // TODO: doesn't work with tabs ('\t')
  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {

    if (cr && (c == ' ' || c == '\t')) {
      offset++;
      continue;
    }

    if (c == ' ' || c == '\t') {
      last_blank_char_pos = i;
    }

    if (
        curr_row_char_count > INPUT_LINE_LIMIT &&
        (last_blank_char_pos / row) < INPUT_LINE_LIMIT &&
        last_blank_char_pos > 0 
      ) {
      // Do not set cr=1, because we continue inserting characters
      s[last_blank_char_pos-offset] = '\n';
      s[i-offset] = c;
      row++;
      curr_row_char_count = i - last_blank_char_pos;
      last_blank_char_pos = -1;
    } else if (
        (curr_row_char_count != 0 &&
                (c == ' ' || c == '\t') &&
                (curr_row_char_count % INPUT_LINE_LIMIT == 0)) ||
        (curr_row_char_count >= INPUT_LINE_LIMIT && last_blank_char_pos > 0 && (c == ' ' || c == '\t'))
      ) {
      s[i-offset] = '\n';
      cr = 1;
      row++;
      curr_row_char_count = i - last_blank_char_pos; // always 0?
      last_blank_char_pos = -1;
    } else if (curr_row_char_count == INPUT_LINE_LIMIT && last_blank_char_pos > 0) {
      s[last_blank_char_pos-offset] = '\n';
      s[i-offset] = c;
      row++;
      curr_row_char_count = i - last_blank_char_pos;
      last_blank_char_pos = -1;
    } else {
      s[i-offset] = c;
      cr = 0;
      curr_row_char_count++;
    }
  }

  if (c == '\n' && (i-offset) != 0) {
    s[i-offset] = c;
    ++i;
  }

  s[i-offset] = '\0';
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