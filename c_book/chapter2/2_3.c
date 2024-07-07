#include <stdio.h>

int atoi2(char s[]) {
  int i, n;

  n = 0;
  for (i = 0; s[i] >= '0' && s[i] <= '9'; ++i) {
    n = 10 * n + (s[i] - '0');
  }

  return n;
}

int htoi(char s[]) {
  int n, i;

  n = 0;
  i = 0;
  if (s[0] == '0' || s[1] == 'x' || s[1] == 'X') {
    i = 1;
  }
  while(s[i] != '\0') {
    if (s[i] >= '0' && s[i] <= '9') {
      n = 16 * n + (s[i] - '0');
    }

    if (s[i] >= 'a' && s[i] <= 'f') {
      n = 16 * n + (s[i] - 'a' + 10);
    }

    if (s[i] >= 'A' && s[i] <= 'F') {
      n = 16 * n + (s[i] - 'A' + 10);
    }

    i++;
  }

  return n;
}

int main() {

  // printf("%d\n", atoi2("123"));
  printf("Hex %x\n", htoi("0xb"));
  printf("Dec %d\n", htoi("b"));
  // printf("%x\n", 0xbb);

  return 0;
}