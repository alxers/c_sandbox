// entab
#include <stdio.h>

#define MAXLINE 100
#define DEFAULT_TAB_LENGTH 3

int getline(char s[], int lim, int default_m, int default_n) {
  int c, i;
  int space_count = 0;
  int non_space_count = 0;
  int last_valid_char_position = -1;
  int tab_col = 0;
  int skip_m = default_m - 1;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    if (c != ' ') {
      s[last_valid_char_position+1] = c;
      last_valid_char_position++;
    }

    if (c == ' ') {
      space_count++;

      if (skip_m) {
        skip_m--;
        s[last_valid_char_position+1] = '*'; // * space
        last_valid_char_position++;
        space_count = 0;
        continue;
      }

      if (space_count != 0 && space_count % DEFAULT_TAB_LENGTH == 0) {
        last_valid_char_position = last_valid_char_position - 2;
        s[last_valid_char_position+1] = '^'; // ^ tab
        last_valid_char_position++;
        space_count = 0;
        skip_m = default_n - 1;
      } else {
        s[last_valid_char_position+1] = '*'; // * space
        last_valid_char_position++;
      }
    }
  }

  // ab**^**^c

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

int main(int argc, char *argv[]) {
  int len;
  char line[MAXLINE];

  int max = 0;

  int default_m = 1;
  int default_n = 4;

  argv++; // Skip first argument (program name)
  argc--;
  if (argc > 0 && **argv == '-') {
    // if argv = ["abc", "def"], then *argv -> "abc", **argv -> 'a'

    // (*argv)++; // Skip '-' sign, "++" prio is higher than "*",
    // withouth parenthesis *argv++ moves char* pointer to the next string (f.e. "def")
    // that is it applies "++" to the argv, then dereference with "*".
    //
    // With parenthesis (*argv)++ first gets the first string from argv
    // f.e. "abc" from ["abc", "def"], then moves pointer
    // one char to the right, so we get "bc"
    (*argv)++;
    default_m = atoi(*argv);
    argv++;
  }

  if (**argv == '+') {
    default_n = atoi(*argv);
  }

  while ((len = getline(line, MAXLINE, default_m, default_n)) > 0) {
  }

  return 0;
}
// end entab

// detab
// #include <stdio.h>

// #define MAXLINE 100
// #define TAB_LEN 3

// int getline(char s[], int lim, int default_m, int default_n) {
//   int c, i;
//   int tab_col = 0;

//   for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
//     if (c == '\t') {
//       int j;
//       tab_col++;

//       if (tab_col == default_m) {
//         for (j = 0; j < TAB_LEN; j++) {
//           // s[i+j] = ' ';
//           s[i+j] = '_';
//         }
//         i = i + (j - 1);
//       } else if (tab_col > default_m && tab_col != 0 && (tab_col - default_m) % default_n == 0) {
//         for (j = 0; j < TAB_LEN; j++) {
//           // s[i+j] = ' ';
//           s[i+j] = '^'; // space
//         }
//         i = i + (j - 1);
//       } else {
//         s[i] = '#'; // tab
//       }

//     } else {
//       s[i] = c;
//     }
//   }

//   if (c == '\n' && i != 0) {
//     s[i] = c;
//     ++i;
//   }

//   s[i] = '\0';
//   printf("%s\n", s);
//   return i;
// }

// void copy(char to[], char from[]) {
//   int i = 0;

//   while((to[i] = from[i]) != '\0') {
//     ++i;
//   }
// }

// int main(int argc, char *argv[]) {
//   int len;
//   char line[MAXLINE];
//   char longest[MAXLINE];

//   int max = 0;
//   int default_m = 1;
//   int default_n = 4;

//   argv++; // Skip first argument (program name)
//   argc--;
//   if (argc > 0 && **argv == '-') {
//     // if argv = ["abc", "def"], then *argv -> "abc", **argv -> 'a'

//     // (*argv)++; // Skip '-' sign, "++" prio is higher than "*",
//     // withouth parenthesis *argv++ moves char* pointer to the next string (f.e. "def")
//     // that is it applies "++" to the argv, then dereference with "*".
//     //
//     // With parenthesis (*argv)++ first gets the first string from argv
//     // f.e. "abc" from ["abc", "def"], then moves pointer
//     // one char to the right, so we get "bc"
//     (*argv)++;
//     default_m = atoi(*argv);
//     argv++;
//   }

//   if (**argv == '+') {
//     default_n = atoi(*argv);
//   }

//   while ((len = getline(line, MAXLINE, default_m, default_n)) > 0) {
//     if (len > max) {
//       max = len;
//       copy(longest, line);
//     }
//   }

//   printf("len: %d, line: %s", max, longest);

//   return 0;
// }
// end detab