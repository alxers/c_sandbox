#include <stdio.h>

#define ARR_LEN 128

int main() {
  int c;

  int sym_len[ARR_LEN];
  for (int i = 0; i < ARR_LEN; i++) {
    sym_len[i] = 0;
  }

  int sym_len_count = 0;
  int max_el = 0;
  while ((c = getchar()) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t') {
      continue;
    }

    if (!(c >= 'A' && c <= 'Z') || !(c >= 'a' && c <= 'z')) {
      sym_len[c]++;
      if (sym_len[c] > max_el) {
        max_el = sym_len[c];
      }
      // printf("c: %d, sym_len[c] %d\n", c, sym_len[c]);
    }
  }

  for (int i = max_el; i > 0; i--) {
    for (int j = 0; j < ARR_LEN; j++) {
      if (sym_len[j]) {
        if (sym_len[j] >= i) {
          printf(" | | ");
        } else {
          printf("     ");
        }
      }
    }
    printf("\n");
  }



  // Print
  // for (int i = 0; i < ARR_LEN; i++) {
  //   if (sym_len[i]) {
  //     printf("%d\n", sym_len[i]);
  //   }
  // }

  return 0;
}