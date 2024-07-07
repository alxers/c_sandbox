// entab
// #include <stdio.h>

// #define MAXLINE 100
// #define DEFAULT_TAB_LENGTH 3

// int getline(char s[], int lim, int tab_length) {
//   int c, i;
//   int space_count = 0;
//   int non_space_count = 0;
//   int last_valid_char_position = -1;

//   for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
//     // printf("%c\n", c);
//     if (c == ' ') {
//       space_count++;
//       if (space_count % tab_length == 0) {
//         s[last_valid_char_position+1] = '\t';
//         last_valid_char_position++;
//         space_count = 0;
//       }
//     } else {
//       if (space_count) {
//         for (int j = 0; j < space_count; j++) {
//           s[last_valid_char_position+1] = ' ';
//           last_valid_char_position++;
//         }
//         if (c != ' ') {
//           s[last_valid_char_position+1] = c;
//           last_valid_char_position++;
//         }
//         space_count = 0;
//       } else {
//         s[last_valid_char_position+1] = c;
//         last_valid_char_position++;
//       }
//     }
//   }

//   if (c == '\n' && last_valid_char_position != 0) {
//     s[last_valid_char_position+1] = c;
//     ++last_valid_char_position;
//   }

//   s[last_valid_char_position+1] = '\0';
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

//   int max = 0;

//   argv++;
//   int tab_length = DEFAULT_TAB_LENGTH;

//   if (*argv != 0) {
//     tab_length = atoi(*argv);
//   }

//   while ((len = getline(line, MAXLINE, tab_length)) > 0) {
//   }

//   return 0;
// }
// end entab

// detab
#include <stdio.h>

#define MAXLINE 100
#define TAB_LEN 3

int getline(char s[], int lim, int tab_length) {
  int c, i;

  for (i = 0; i < lim - 1 && (c = getchar()) != EOF && c != '\n'; ++i) {
    if (c == '\t') {
      int j;
      for (j = 0; j < tab_length; j++) {
        s[i+j] = ' ';
      }
      i = i + (j - 1);
    } else {
      s[i] = c;
    }
  }

  if (c == '\n' && i != 0) {
    s[i] = c;
    ++i;
  }

  s[i] = '\0';
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
  char longest[MAXLINE];

  int max = 0;

  argv++;
  int tab_length = TAB_LEN;

  if (*argv != 0) {
    tab_length = atoi(*argv);
  }

  while ((len = getline(line, MAXLINE, tab_length)) > 0) {
    if (len > max) {
      max = len;
      copy(longest, line);
    }
  }

  printf("len: %d, line: %s", max, longest);

  return 0;
}
// end detab