#include <stdio.h>

#include "int.h"
#include "Xorshift32.h"
#include "Random.h"

int main(void) {
   Xorshift32 x32 = Xorshift32_init(1);
   std_Random rng = Xorshift32_random(&x32);
   for (int i = 100; i--; )
      printf("%d %d\n", i, std_Random_uintLessThanBiased(&rng, u16, 59049));
}
