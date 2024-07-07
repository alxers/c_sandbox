#include <stdio.h>

#define MAXLEN 100000
// test
int main() {
  char result[MAXLEN];
  int c;

  int i = 0;
  int counter = 0;

  int single_line_comment = 0;
  int multi_line_comment[2];
  multi_line_comment[0] = -1;
  multi_line_comment[1] = -1;

  int double_quotes_counter = 0;
  int single_quotes_counter = 0;
  int escape_char = 0;

  while ((c = getchar()) != EOF) {
    counter++;
    // TODO: when i = 0 then result[i-n] still doesn't fail
    // because in C there is always (?) some random value
    // How bad is it? Should we have result[i-1] && result[i-1] != ... instead?
    if (c == '"' && single_quotes_counter != 1 && !escape_char) {
      double_quotes_counter++;
    }
    // test
    if (c == '\'' && double_quotes_counter != 1 && !escape_char) {
      single_quotes_counter++;
    }
    // Check for escape char should be below
    // checks for quotes
    if (c == '\\' && !escape_char) {
      escape_char = 1;
    } else {
      escape_char = 0;
    }
    // test 1.5
    if (!single_line_comment && multi_line_comment[1] != 1 && (double_quotes_counter == 1 || single_quotes_counter == 1)) {
      result[i] = c;
      i++;
    } else {
      if (double_quotes_counter) {
        double_quotes_counter = 0;
      }
      if (single_quotes_counter) {
        single_quotes_counter = 0;
      }
      // For single line comments
      if (i != 0 && c == '/' && result[i-1] == '/' && multi_line_comment[1] != 1) {
        i--;
        single_line_comment = 1;
      }

      if (c == '\n') {
        single_line_comment = 0;
      }

      /* test_multi //* \ \\ \* \/ // *\ */

      if (c == '*' && multi_line_comment[1] == 1) {
        multi_line_comment[0] = counter;
      }
      if (c == '/' && (counter - multi_line_comment[0] == 1) && multi_line_comment[1] == 1) {
        multi_line_comment[1] = 0;
        continue;
      }

      if (c == '/' && multi_line_comment[1] != 1) {
        multi_line_comment[0] = counter;
      }
      if (c == '*' && (counter - multi_line_comment[0]) == 1) {
        multi_line_comment[1] = 1;
        i--;
        result[i] = '\0';
        continue;
      }

      // test3
      if (single_line_comment || multi_line_comment[1] == 1) {
        continue;
      } else {
        result[i] = c;
        i++;
      }

    }
  }
  // test 4
  result[i] = '\0';
  printf("\nResult: %s\nCounter: %d\n", result, i);
  // comment
  /* " /* " '//\' '*' * / / *   */
  // /* */
  return 0;
}

// #include <stdio.h>

// #define MAXLEN 100000
// // test
// int main() {
//   char result[MAXLEN];
//   int c;

//   int i = 0;
//   int comment = 0;
//   int double_quotes_counter = 0;
//   int single_quotes_counter = 0;

//   while ((c = getchar()) != EOF) {
//     // TODO: when i = 0 then result[i-n] still doesn't fail
//     // because in C there is always (?) some random value
//     // How bad is it? Should we have result[i-1] && result[i-1] != ... instead?
//     if (c == '"' && result[i-1] != '\\' && result[i-2] != '\"' && single_quotes_counter != 1) {
//       double_quotes_counter++;
//     }
//     // test
//     if (c == '\'' && double_quotes_counter != 1) {
//       if (result[i-1] == '\\' && result[i-2] == '\'') {
//         // do nothing
//       } else if (result[i-1] == '\\' && result[i-2] == '\\') {
//         single_quotes_counter++;
//       } else {
//         single_quotes_counter++;
//       }
//     }
//     // test 1.5
//     if (!comment && (double_quotes_counter == 1 || single_quotes_counter == 1)) {
//       result[i] = c;
//       i++;
//     } else {
//       if (double_quotes_counter) {
//         double_quotes_counter = 0;
//       }
//       if (single_quotes_counter) {
//         single_quotes_counter = 0;
//       }
//       // test2
//       if (i != 0 && c == '/' && result[i-1] == '/') {
//         i--;
//         comment = 1;
//       }

//       if (c == '\n') {
//         comment = 0;
//       }
//       // test3
//       if (comment) {
//         continue;
//       } else {
//         result[i] = c;
//         i++;
//       }
//     }
//   }
//   // test 4
//   result[i] = '\0';
//   printf("\nResult: %s\nCounter: %d\n", result, i);
//   // comment
//   return 0;
// }