// Vertical
#include <stdio.h>

int main() {
  int c;
  int nw = 0;
  int curr_word_len = 0;

  int word_len[10];
  for (int i = 0; i < 10; i++) {
    word_len[i] = 0;
  }

  int word_len_count = 0;
  while ((c = getchar()) != EOF) {

    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
      ++curr_word_len;
    } else {
      // printf("%d\n", curr_word_len);
      word_len[word_len_count++] = curr_word_len;
      curr_word_len = 0;
    }
  }

  // Find max element in an array
  // it's the amount of rows for the histogram
  int max_el = 0;
  for (int i = 0; i < 10; i++) {
    if (word_len[i] && word_len[i] > max_el) {
      max_el = word_len[i];
    }
  }
  // printf("%d\n", max_el);

  for (int i = max_el; i > 0; i--) {
    for (int j = 0; j < 10; j++) {
      if (word_len[j]) {
        if (word_len[j] >= i) {
          if (word_len[j] == i) {
            // printf("--\n");
            printf("|-|");
          } else {
            printf("| |");
          }
        } else {
          printf("   ");
        }
      }
    }
    printf("\n");
  }

  return 0;
}

// Horizontal
// #include <stdio.h>
// int main() {
//   int c;
//   int nw = 0;
//   int curr_word_len = 0;

//   int word_len[10];
//   for (int i = 0; i < 10; i++) {
//     word_len[i] = 0;
//   }

//   int word_len_count = 0;
//   while ((c = getchar()) != EOF) {

//     if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
//       ++curr_word_len;
//     } else {
//       // printf("%d\n", curr_word_len);
//       word_len[word_len_count++] = curr_word_len;
//       curr_word_len = 0;
//     }
//   }

//   // Horizontal rows
//   for (int i = 0; i < 10; i++) {
//     if (word_len[i]) {
//       printf("---\n");

//       for (int j = 0; j < word_len[i]; j++) {
//         printf("| |\n");
//       }
//     }
//   }
//   printf("---\n");


//   return 0;
// }