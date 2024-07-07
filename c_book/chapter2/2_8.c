#include <stdio.h>

unsigned rightrot(unsigned x, int n) {
  // Take n bits and rotate them to the right, f.e.
  // 000011 >> 2 would give 110000
  unsigned righmost_n_ones = ~(~0 << n);
  unsigned righmost_n_bits_from_x = x & righmost_n_ones;
  unsigned n_shifted_x = x >> n;

  unsigned lefmost_n_mask = righmost_n_bits_from_x << 32-n;
  // unsigned nullify_left_n_mask;
  unsigned result = n_shifted_x | lefmost_n_mask;

  return result;
}

int main() {
  // 19 is 0b00000000000000000000000000010011, result = 0b11000000000000000000000000000100
  printf("%u\n", rightrot(19, 2));

  //19 is 0b00000000000000000000000000010011, result = 0b10011000000000000000000000000000
  printf("%u\n", rightrot(19, 5));

  //1 is 0b00000000000000000000000000000001, result = 0b10000000000000000000000000000000
  printf("%u\n", rightrot(1, 1));
  return 0;
}