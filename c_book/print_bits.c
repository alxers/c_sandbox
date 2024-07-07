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
    
    unsigned x = 25;
    unsigned y = 21;
    int p = 3;
    int n = 3;

    unsigned right_n_bits_as_ones = ~(~0 << n);
    unsigned rightmost_n_from_y = y & right_n_bits_as_ones;
    unsigned y_mask = rightmost_n_from_y << (p+1-n);
    unsigned result = x | y_mask;
    
    unsigned f = y_mask; 
    printf("double: %f\n", f);
    for (int i = sizeof(f) * 8 - 1; i >= 0; i--)
    {
        printf("%d", printBit(&f, i));
    }
    printf("\n");
}
