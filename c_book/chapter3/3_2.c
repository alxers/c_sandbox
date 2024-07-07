#include <stdio.h>

void escape(char s[], char t[]) {
  int i;
  int j = 0;
  for (i = 0; t[i] != '\0'; i++) {
    switch(t[i]) {
      case '\n':
        s[j] = '\\';
        j++;
        s[j] = 'n';
        break;
      case '\t':
        s[j] = '\\';
        j++;
        s[j] = 't';
        break;
      default:
        s[j] = t[i];
        break;
    }
    j++;
  }
  s[j] = '\0';
}

void unescape(char s[], char t[]) {
  int i;
  int j = 0;
  for (i = 0; t[i] != '\0'; i++) {
    switch(t[i]) {
      case 'n':
        if (t[i-1] == '\\') {
          j--;
          s[j] = '\n';
          break;
        }
      case 't':
        if (t[i-1] == '\\') {
          j--;
          s[j] = '\t';
          break;
        }
      default:
        s[j] = t[i];
        break;
    }
    j++;
  }
  s[j] = '\0';
}

int main() {
  char s[] = "";
  char t[] = "te\nst\ta\\";
  escape(s, t);
  printf("%s\n", s);

  char ns[] = "";
  char nt[] = "te\\nst\\ta\\";
  unescape(ns, nt);
  printf("%s\n", ns);
  printf("\n");
  // printf("%s\n", nt);
  return 0;
}