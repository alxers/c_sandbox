#include <stdio.h>

int strindex(char s[], char t[]) {
  int s_len = 0;
  int t_len = 0;
  int counter = 0;

  for (int l = 0; s[l] != '\0'; l++) {
    s_len++;
  }

  for (int l = 0; t[l] != '\0'; l++) {
    t_len++;
  }

  for (int i = s_len-1; i >= 0; i--) {
    for (int j = t_len-1; j >= 0; j--) {
      if (s[i] == t[j]) {
        counter++;
        break;
      }
    }

    if (counter == t_len) {
      return i;
    }
  }

  return -1;
}

int main() {
  char s[] = "Helelo";
  char t[] = "el";
  // char s[] = "el";
  // char t[] = "el";
  printf("%d\n", strindex(s, t));
  return 0;
}