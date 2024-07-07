#include <stdio.h>

/*

n = n & 0177; // This will nullify 7 low-order bits of "n".

The value of an integer can be specified in octal or hexadecimal instead of decimal. A leading
0 (zero) on an integer constant means octal; a leading 0x or 0X means hexadecimal

So 0177 is an octal value. It's 127 in decimal
Which is 01111111 in binary.

*/

unsigned getbits(unsigned x, int p, int n) {
  return (x >> (p+1-n)) & ~(~0 << n);
}

unsigned setbits(unsigned x, int p, int n, unsigned y) {
  unsigned right_n_bits_as_ones = ~(~0 << n);
  // Take a mask that is all 0's except the rightmost n bits
  // Shift this non zero bits to the left and invert
  // Thus we have something like 111100111
  // This is used to nullify bits in "x"
  unsigned nullify_mask = ~(right_n_bits_as_ones << (p+1-n));
  x = x & nullify_mask;

  unsigned rightmost_n_from_y = y & right_n_bits_as_ones;
  unsigned y_mask = rightmost_n_from_y << (p+1-n);
  unsigned result = x | y_mask;
  return result;
}

int main() {

  // printf("%d\n", getbits(10, 3, 3)); // 10 is b1010, take 3 bits from 4th, it's 101, which is 5
  // printf("%d\n", getbits(6, 2, 2)); // 6 -> 3
  printf("%ud\n", setbits(25, 3, 3, 21)); // 25 is 0b11001, 21 is 0b10101, p=3,n=3, result=0b11011 = 27
  printf("%ud\n", setbits(5, 2, 2, 2)); // 5 is b101, 2 is b10, p=2,n=2, result = 0b101 = 5
  printf("%ud\n", setbits(7, 2, 2, 2)); // 7 is b111, 2 is b10, p=2,n=2, result = 0b101 = 5

  printf("%ud\n", setbits(5, 2, 3, 0)); // 5 is b101, 0 is b0, p=2,n=3, result = 0b000 = 0
  printf("%ud\n", setbits(0, 1, 2, 3)); // 3 is b11, 0 is b0, p=1,n=2, result = 0b011 = 3

  printf("%ud\n", setbits(~0, 1, 2, ~0)); // ~0 is all bits = 1, p=1,n=2, result all bits = 1

  return 0;
}