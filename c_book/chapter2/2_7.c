#include <stdio.h>


// Invert n bits from p position (counting from the mostright as zero)
unsigned invert(unsigned x, int p, int n) {
  unsigned right_p_bits_as_ones = ~(~0 << p+1);
  unsigned right_p_bits_from_x = x & right_p_bits_as_ones;
  unsigned right_n_bits_as_zeros = (~0 << n);

  unsigned n_bits_at_p_mask = right_p_bits_as_ones & right_n_bits_as_zeros;
  unsigned invert_mask = ~(right_p_bits_from_x & right_n_bits_as_zeros) & n_bits_at_p_mask;
  // unsigned invert_mask_stripped_from_left_and_right = invert_mask & 

  // Invert mask inverts n bits from p position (starting from the rightmost as 0)
  // Now we need to set x bits at these positions
  unsigned right_n_bits_as_ones = ~(~0 << n);
  // unsigned nullify_mask = ~(right_n_bits_as_ones);
  unsigned nullify_mask = ~(right_n_bits_as_ones << (p+1-n));
  x = x & nullify_mask;
  unsigned result = x | invert_mask;
  return result;
}

int main() {
  printf("%ud\n", invert(5, 3, 2)); // 5 is 0b0101, invert from p=3, n=2 bits, result = 0b1001 = 9
  printf("%ud\n", invert(3, 1, 2)); // 3 is 0b0011, invert from p=2, n=2 bits, result = 0b0000 = 0
  printf("%ud\n", invert(0, 3, 1)); // 3 is 0b0000, invert from p=3, n=1 bits, result = 0b1000 = 8
  printf("%ud\n", invert(15, 3, 3)); // 15 is 0b1111, invert from p=3, n=3 bits, result = 0b0001 = 1
  printf("%ud\n", invert(15, 3, 1)); // 15 is 0b1111, invert from p=2, n=2 bits, result = 0b0111 = 7
  return 0;
}