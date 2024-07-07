#include <stdio.h>

#include <limits.h>
#include <float.h>

//remove
#include <stdint.h>

int main() {

  // 2.1 part 1, printing constant values from #inludes
  printf("Char bit %d\n", CHAR_BIT);
  printf("Char max %d\n", CHAR_MAX);
  printf("Char min %d\n", CHAR_MIN);
  printf("Unsigned char max %d\n", UCHAR_MAX);

  printf("Short min %d\n", SHRT_MIN);
  printf("Short max %d\n", SHRT_MAX);
  printf("Unsigned short max %d\n", USHRT_MAX);

  printf("Int min %d\n", INT_MIN);
  printf("Int max %d\n", INT_MAX);
  printf("Unsigned int max %u\n", UINT_MAX);

  printf("Long min %ld\n", LONG_MIN);
  printf("Long max %ld\n", LONG_MAX);
  printf("Unsigned long max %lu\n", ULONG_MAX);

  printf("\n\n");

  // Floats
  printf("Float radix (or base of the exponent) %d\n", FLT_RADIX);
  printf("FLT_ROUNDS floating-point rounding mode for addition %d\n", FLT_ROUNDS);
  printf("FLT_DIG decimal digits of precision %d\n", FLT_DIG);
  printf("FLT_EPSILON smallest number x such that 1.0+x != 1.0 %.9f\n", FLT_EPSILON);
  printf("FLT_MANT_DIG number of base FLT_RADIX in mantissa %d\n", FLT_MANT_DIG);
  printf("Float max %.9f\n", FLT_MAX);
  printf("FLT_MAX_EXP maximum n such that FLT_RADIX^(n-1) is representable %d\n", FLT_MAX_EXP);
  printf("FLT_MIN minimum normalized floating-point number %.9f\n", FLT_MIN);
  printf("FLT_MIN_EXP minimum n such that 10^n is a normalized number %d\n", FLT_MIN_EXP);
  printf("\n");
  printf("DBL_DIG decimal digits of precision %d\n", DBL_DIG);
  printf("DBL_EPSILON smallest number x such that 1.0+x != 1.0 %.50f\n", DBL_EPSILON);
  printf("DBL_MANT_DIG number of base DBL_RADIX in mantissa %d\n", DBL_MANT_DIG);
  printf("DBL max %.50f\n", DBL_MAX);
  printf("DBL_MAX_EXP maximum n such that DBL_RADIX^(n-1) is representable %d\n", DBL_MAX_EXP);
  printf("DBL_MIN minimum normalized floating-point number %.150f\n", DBL_MIN);
  printf("DBL_MIN_EXP minimum n such that 10^n is a normalized number %d\n", DBL_MIN_EXP);

  printf("\n\n");

  // 2.1 part 2, calculating values manually

  // Char
  char schar;
  unsigned char uchar;

  // Unsigned char
  uchar = 0;
  uchar--; // causes to overflow and gives max value?
  printf("Unsigned char from %d to %u\n", 0, uchar);

  // Signed char
  schar = uchar / 2;
  printf("Signed char from %d to %d\n", -schar, schar);


  // Int
  int sintr;
  unsigned int uintr;

  // Unsigned int
  uintr = 0;
  uintr--; // causes to overflow and gives max value?
  printf("Unsigned int from %d to %u\n", 0, uintr);

  // Signed int
  sintr = uintr / 2;
  printf("Signed int from %d to %d\n", -sintr, sintr);

  // Same for others

  // Float
  // Floating point is represented as V = x * 2^y
  // IEEE format V = (-1)^s * M * 2^E
  // where s is sign bit (s = 1 or s = 0), M is a fractional number
  // and E is an exponent
  //
  // Bit representation:
  // s = 1 bit
  // k bit exponent (float k = 8, double k = 11)
  // n bit fraction (float n = 23, double n = 52)

  // There are 3 cases
  // 1: normalized values, when exp bits are neither all 0s nor 1s (255 for
  // single precision and 2047 for double)
  // In this case exponent value E = e - Bias, where Bias = 2^(k-1) - 1.
  // This yields exponent ranges from -126 to +127 for single precision and
  // -1022 to +1023 for double.
  //
  // The fraction is a value 0 <= f < 0, having binary representation 0.f_(n-1)...f_1f_0.
  // The significand M = 1 + f (implied leading 1 representation)
  //
  // 2: denormalized values, when exp bits are all 0s
  // in this case E = 1 - Bias and significand M = f (without implied leading 1).
  //
  // 3: special values, when exp bits are all 1s. If frac all 0s, then it's an Infinity,
  // otherwise NaN

  // To calculate a single precision max value:
  // Set exp to a maximum value for a normalized float, that is 8 bits 1111 1110
  // which is 255 and Bias = 2^(k-1) - 1 = 2^(8-1) - 1 = 127,
  // so E = e - Bias, E = 254 - 127 = 127
  // For the frac part, we set all 23 bits to 1s, then
  // it's 1111 1111 1111 1111 1111 111 = 0.99999988079071044921875
  // for normalized values M = 1 + f, then the actual value is
  // 1.99999988079071044921875
  // Thus to get max float values we need to multiply 2^E * M
  // 2^127 * 1.99999988079071044921875
  float sfloat = 1.99999988079071044921875;
  for (int i = 1; i <= 127; i++) {
    sfloat *= 2; 
  }
  // printf("Float max %0.15f\n", FLT_MAX);
  printf("Float max %0.15f\n", sfloat);

  // To min value of single precision is just a negative max value.


  // For double precision the process is the same except that
  // k = 11, n = 52
  // e = 1111 1111 110 = 2046
  // Thus E = e - Bias, e = 2046 - 2^(11-1) - 1 = 2046 - 1023 = 1023
  // M = 52bits set to 1 which is 0.9999999999999997779553950749686919152736663818359375
  // and since it's normalized, M = 1 + frac = 
  // = 1.9999999999999997779553950749686919152736663818359375
  // 2^E * M is 2^1023 * 1.9999999999999997779553950749686919152736663818359375
  double dfloat = 1.9999999999999997779553950749686919152736663818359375;
  for (int i = 1; i <= 1023; i++) {
    dfloat *= 2; 
  }
  printf("Double max %0.15f\n", DBL_MAX);
  printf("Double max %0.15f\n", dfloat);

  return 0;
}


/*

To print bits, also see https://godbolt.org/z/3o1xdP888

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

int printBit(void *buff, int i) 
{
    unsigned char *data = buff;
    return !!(data[i / 8] & (1 << (i & 7)));
}


int main(void)
{

    double f = 1.9999999999999997779553950749686919152736663818359375;
    printf("double: %f\n", f);
    for (int i = sizeof(f) * 8 - 1; i >= 0; i--)
    {
        printf("%d", printBit(&f, i));
    }
    printf("\n");
}


*/