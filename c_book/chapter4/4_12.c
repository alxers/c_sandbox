#include <stdio.h>

void itoa(int num, char s[], int iter, char tmp_s[], int is_minus) {
  if (num < 0) {
    num = -num;
    is_minus = 1;
  }
  if (!num) {
    int i = 0;
    if (is_minus) {
      tmp_s[0] = '-';
      iter++;
      i++;
    }
    for (; i < iter; i++) {
      tmp_s[i] = s[iter-i-1];
    }
    // s = tmp_s; // doesn't work, so I had to add tmp_s as a workaround
    // s[iter] = '\0';
    return;
  }
  int modulo_num = num % 10;
  s[iter] = modulo_num + '0';
  itoa(num / 10, s, ++iter, tmp_s, is_minus);
}

int main() {
  // int iter = 0;
  char s[100];
  char tmp_s[100];
  for (int i = 0; i < 100; i++) {
    s[i] = 0;
    tmp_s[i] = 0;
  }
  itoa(-100, s, 0, tmp_s, 0);

  printf("%s\n", tmp_s);
  return 0;
}