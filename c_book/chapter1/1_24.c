#include <stdio.h>

#define MAXLEN 10000

#define PAREN 1
#define SQUARE_BR 2
#define CURLY_BR 3
#define ANGLE_BR 4

#define INCLUDE_LEN 8

/*
Round or parentheses with symbol ()

Square or box brackets with symbol []

Braces or curly brackets with symbol {}

Angle brackets <>
*/

int abs(int n) {
  if (n >= 0) {
    return n;
  }
  return -n;
}

int main() {
  int c;
  int prev_char = 0;
  int curr_char = 0;
  int char_counter = 0;

  // Comments
  int single_line_comment = 0;
  int multi_line_comment = 0;

  // Braces
  int parentheses = 0;
  int square_brackets = 0;
  int curly_braces = 0;
  int angle_brackets = 0;

  // Quotes
  int single_quotes_counter = 0;
  int double_quotes_counter = 0;

  // Escape
  int escape_char = 0;

  // Used to match opened/closed parentheses/brackets/braces
  int opened_paren[MAXLEN];
  int closed_paren[MAXLEN];

  int opened_paren_counter = 0;
  int closed_paren_counter = 0;

  for (int m = 0; m < MAXLEN; m++) {
    opened_paren[m] = 0;
    closed_paren[m] = 0;
  }

  // Check for #include in order to validate angle brackets "<" and ">"
  int include_counter = 0;
  int include_flag = 0;
  int include_arr[INCLUDE_LEN] = { '#', 'i', 'n', 'c', 'l', 'u', 'd', 'e' };
  int include_checker_arr[INCLUDE_LEN];
  for (int p = 0; p < INCLUDE_LEN; p++) {
    include_checker_arr[p] = 0;
  }

  // Ctrl + Z is EOF in windows
  while ((c = getchar()) != EOF) {
    char_counter++;

    prev_char = curr_char;
    curr_char = c;

    // Escape
    // if (prev_char == '\\' && !escape_char) {
    //   escape_char = 1;
    // } else {
    //   escape_char = 0;
    // }
    if (prev_char == '\\') {
      // Ignore any char that comes after escape char (backward slash)
      curr_char = 0;
      continue;
    }

    // Quotes
    if (!single_line_comment && !multi_line_comment) {
      if (curr_char == '\'' && double_quotes_counter % 2 == 0 && !escape_char) {
        single_quotes_counter++;
      }

      if (curr_char == '"' && single_quotes_counter % 2 == 0 && !escape_char) {
        double_quotes_counter++;
      }
    }

    if (single_quotes_counter % 2 == 0 && double_quotes_counter % 2 == 0) {

      // Comments
      if (prev_char == '/' && curr_char == '/') {
        single_line_comment = 1;
      }
      if (single_line_comment && curr_char == '\n') {
        single_line_comment = 0;
      }
      if (prev_char == '/' && curr_char == '*') {
        multi_line_comment = 1;
      }
      if (prev_char == '*' && curr_char == '/') {
        multi_line_comment = 0;
      }

      if (!single_line_comment && !multi_line_comment) {

        // Check if there's been #include word
        if (include_counter < INCLUDE_LEN) {
          include_checker_arr[include_counter] = curr_char;
          include_counter++;
        } else {
          include_counter = 0;
        }

        for (int z = 0; z < INCLUDE_LEN; z++) {
          if (include_checker_arr[z] != include_arr[z]) {
            include_flag = 0;
            break;
          } else {
            include_flag = 1;
          }
        }

        if (curr_char == '\n') {
          include_flag = 0;
        }

        // Parenteses
        if (c == '(') {
          parentheses++;
          opened_paren[char_counter] = PAREN;
          // opened_paren_counter++;
        }
        if (c == ')') {
          parentheses++;
          closed_paren[char_counter] = PAREN;
          // closed_paren_counter++;
        }

        // Square brackets
        if (c == '[') {
          square_brackets++;
          opened_paren[char_counter] = SQUARE_BR;
          // opened_paren_counter++;
        }
        if (c == ']') {
          square_brackets++;
          closed_paren[char_counter] = SQUARE_BR;
          // closed_paren_counter++;
        }

        // Curly braces
        if (c == '{') {
          curly_braces++;
          opened_paren[char_counter] = CURLY_BR;
          // opened_paren_counter++;
        }
        if (c == '}') {
          curly_braces++;
          closed_paren[char_counter] = CURLY_BR;
          // closed_paren_counter++;
        }

        // Angle brackets
        if (include_flag) {
          if (c == '<') {
            angle_brackets++;
            opened_paren[char_counter] = ANGLE_BR;
            // opened_paren_counter++;
          }
          if (c == '>') {
            angle_brackets++;
            closed_paren[char_counter] = ANGLE_BR;
            // closed_paren_counter++;
          }
        }
      }

    }
  }

  if (single_quotes_counter % 2 != 0 || double_quotes_counter % 2 != 0) {
    printf("Error: unmatched quotes\n");
  }

  if (multi_line_comment) {
    printf("Error: multi line comment is not closed\n");
  }

  if (
    parentheses % 2 != 0 ||
    square_brackets % 2 != 0 ||
    curly_braces % 2 != 0 ||
    angle_brackets % 2 != 0
  ) {
    printf("Error: uneven amount of parens\n");
  }

  // for (int a = 0; a < MAXLEN; a++) {
  //   if (opened_paren[a]) {
  //     for (int b = 0; b < MAXLEN; b++) {
  //       if (closed_paren[b] && b < a && closed_paren[b] == opened_paren[a]) {
  //         printf("Error: unmatched paren\n");
  //       }
  //       if (closed_paren[b] && closed_paren[b] != opened_paren[a]) {
  //         printf("Error: unmatched paren\n");
  //       }
  //     } 
  //   }
  // }

  int diff = 0;
  for (int a = 0; a < MAXLEN; a++) {
    if (opened_paren[a] && !diff) {
      for (int b = 0; b < MAXLEN; b++) {
        if (closed_paren[b] && b < a) {
          printf("Error: closed paren before open paren\n");
          break;
        }
        if (closed_paren[b]) {
          diff = abs(b - a);
          break;
        }
      }
    }

    if (diff && opened_paren[a] && closed_paren[a+diff] && (opened_paren[a] != closed_paren[a+diff])) {
      printf("Error: unmatched parens\n");
      break;
    }
  }

  // Tests
  // ({[})]
  // ({[]
  // {}[]
  // ' hello
  // "hello
  // \' hello '
  // \//
  /*
    Broken code to test

    if (a) {
      '
    }
  */

  return 0;
}