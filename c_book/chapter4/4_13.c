#include <stdio.h>

void reverse(char s[], int str_len, int iter, char tmp_s[]) {
  if (str_len < 0) {
    tmp_s[iter] = '\0';
    return;
  }

  tmp_s[iter] = s[str_len];
  iter++;
  str_len--;
  reverse(s, str_len, iter, tmp_s);
}

int main() {
  char s[] = "Hello";
  char tmp_s[5];
  for (int i = 0; i < 5; i++) {
    // s[i] = 0;
    tmp_s[i] = 0;
  }

  reverse(s, 4, 0, tmp_s);
  printf("%s\n", tmp_s);
  return 0;
}