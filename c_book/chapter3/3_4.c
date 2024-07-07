#include <stdio.h>
#include <string.h>
#include <limits.h>

void reverse(char s[]) {
  int c, i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

// Original itoa with error
// void itoa(int n, char s[]) {
//   int i, sign;
//   if ((sign = n) < 0) {
//     n = -n;
//   }

//   i = 0;

//   do {
//     s[i++] = n % 10 + '0';
//   } while ((n /= 10) > 0);

//   if (sign < 0) {
//     s[i++]  = '-';
//   }
//   s[i] = '\0';
//   reverse(s);
// }

// Bad, but works?
// void itoa(int n, char s[]) {
//   int i, sign;
//   int flag;
//   int bit_flip = !(n % 10);

//   if (n < 0) {
//     sign = -1;

//     if (!bit_flip) {
//       n = ~n;
//       flag = 1; 
//     } else {
//       n = -n;
//     }
//   } else {
//     sign = 0;
//   }

//   i = 0;

//   do {
//     if (sign < 0 && flag) {
//       s[i++] = n % 10 + '0' + 1;
//       flag = 0;
//     } else {
//       s[i++] = n % 10 + '0';
//     }
//   } while ((n /= 10) > 0);

//   if (sign < 0) {
//     s[i++]  = '-';
//   }
//   s[i] = '\0';
//   reverse(s);
// }

// Better option
void itoa(int n, char s[]) {
  int i, sign = 0;

  i = 0;
  int num;

  do {
    num = n % 10;
    if (num < 0) {
      sign = -1;
      num = -num;
    }
    s[i++] = num + '0';
  } while ((n /= 10) != 0);

  if (sign < 0) {
    s[i++]  = '-';
  }
  s[i] = '\0';
  reverse(s);
}


int main() {
  // Maximum signed integer is 2147483647,
  // minumum signed is -2147483648
  // When this part is run with -2147483648
  // if ((sign = n) < 0) {
  //   n = -n;
  // }
  //
  // it overflows when the sign is changed

  // printf("Int min %d\n", INT_MIN);
  // printf("Int max %d\n", INT_MAX);

  char s[100];
  itoa(-2147483648, s);
  // itoa(-2147483647, s);
  // itoa(-100, s);
  // itoa(2147483647, s);
  // itoa(0, s);
  // itoa(-123, s);
  printf("%s\n", s);
  return 0;
}