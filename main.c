#include <stdio.h>
#include <stdlib.h>

/*
 * Get the n bits of x starting from position p
 */
unsigned get_bits(unsigned x, unsigned p, unsigned n)
{
  // Create a mask of 1 bits of length n starting at position 0
  unsigned mask = ~(~0 << n);

  // Shift the end of the desired range of bits to position 0
  x = x >> (p + 1 - n);

  // Zero out the bits left of the starting position
  x = x & mask;

  return x;
}


/*
 * Set n bits of x from position p to the rightmost n bits of y
 */
void set_bits(unsigned *x, int p, int n, unsigned y)
{
  // Create a mask of 1 bits of length n starting at position 0
  unsigned mask = ~(~0 << n);

  // Zero out the bits to set in x
  *x = *x & ~(mask << (p + 1 - n));

  // Shift the bits in y and set them in x
  *x = *x | ((y & mask) << (p + 1 - n));
}


int main(int argc, char ** argv)
{
  return 0;
}
