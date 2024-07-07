#include <stdio.h>

void expand(char s1[], char s2[]) {
  int c;
  int l = 0;
  int n = 0;

  char chars[5];
  int a;
  for (a = 0; a < 5; a++) {
    chars[a] = 0;
  }
  chars[a-1] = '\0';

  int char_counter = 0;
  for (int j = 0; s1[j] != '\0'; j++) {
    if (
      (s1[j] >= 'a' && s1[j] <= 'z' && s1[j+1] == '-' && s1[j+2] >= 'a' && s1[j+2] <= 'z') ||
      (s1[j] >= 'A' && s1[j] <= 'Z' && s1[j+1] == '-' && s1[j+2] >= 'A' && s1[j+2] <= 'Z')
    ) {
      chars[char_counter++] = s1[j];
      chars[char_counter++] = s1[j+2];
    } else if ((s1[j] >= '0' && s1[j] <= '9' && s1[j+1] == '-' && s1[j+2] >= '0' && s1[j+2] <= '9')) {
      chars[char_counter++] = s1[j];
      chars[char_counter++] = s1[j+2];
    }
  }


  c = chars[0];
  for (l = 0; l <= (chars[1] - chars[0]); l++) {
    s2[l] = c;
    c++;
  }
  if (chars[3]) {
    c = chars[2];
    for (n = 0; n <= (chars[3] - chars[2]); n++) {
      s2[l+n] = c;
      c++;
    } 
  }

  s2[l+n] = '\0';
}

int main() {
  char s1[] = "a-z";
  char s2[1000];
  expand(s1, s2);
  printf("1)a-z: %s\n", s2);
  printf("\n");

  char s3[] = "a-b-c";
  char s4[1000];
  expand(s3, s4);
  printf("2)a-b-c: %s\n", s4);
  printf("\n");

  char s5[] = "-a-z-";
  char s6[1000];
  expand(s5, s6);
  printf("3)-a-z-: %s\n", s6);
  printf("\n");

  char s7[] = "A-Z";
  char s8[1000];
  expand(s7, s8);
  printf("4)A-Z: %s\n", s8);
  printf("\n");

  char s9[] = "-A-z-";
  char s10[1000];
  expand(s9, s10);
  printf("5)-A-z-: %s\n", s10);
  printf("\n");

  char s11[] = "0-9";
  char s12[1000];
  expand(s11, s12);
  printf("6)0-9: %s\n", s12);
  printf("\n");

  char s13[] = "a-z0-9";
  char s14[1000];
  expand(s13, s14);
  printf("7)a-z0-9: %s\n", s14);
  printf("\n");

  char s15[] = "a--";
  char s16[1000];
  expand(s15, s16);
  printf("8)a--: %s\n", s16);
  printf("\n");

  char s17[] = "--z";
  char s18[1000];
  expand(s17, s18);
  printf("9)--z: %s\n", s18);
  printf("\n");

  char s19[] = "-b-k-";
  char s20[1000];
  expand(s19, s20);
  printf("10)-b-k-: %s\n", s20);
  printf("\n");

  char s21[] = "-0-9A-Z";
  char s22[1000];
  expand(s21, s22);
  printf("11)-0-9A-Z: %s\n", s22);
  printf("\n");

  return 0;
}