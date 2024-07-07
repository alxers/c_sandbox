#include <stdio.h>
#include <ctype.h>

double atof(char s[]) {
  double val, power;
  int i, sign;

  for (i = 0; isspace(s[i]); i++)
    ;

  sign = (s[i] == '-') ? -1 : 1;
  if (s[i] == '+' || s[i] == '-') {
    i++;
  }

  for (val = 0.0; isdigit(s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
  }

  if (s[i] == '.') {
    i++;
  }

  for (power = 1.0; isdigit(s[i]); i++) {
    val = 10.0 * val + (s[i] - '0');
    power *= 10;
  }

  int e_sign = 1;
  if (s[i] == 'e') {
    i++;
    if (s[i] == '-') {
      e_sign = -1;
      i++;
    }
  }

  int tenth_pow = 0;
  for (; isdigit(s[i]); i++) {
    tenth_pow = 10.0 * tenth_pow + (s[i] - '0');
  }

  for (int p = 0; p < tenth_pow; p++) {
    if (e_sign < 0) {
      power *= 10;
    } else {
      power /= 10;
    }
  }

  return sign * val / power;
}

int main() {
  // char s[] = "123.15";
  char s[] = "123.15e2"; // 123.15 * 10^(2) = 12315
  // char s[] = "123.15e-2"; // 123.15 * 10^(-2) = 1.2315
  printf("%f\n", atof(s));
  return 0;
}