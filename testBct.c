#include "int.h"
#include "opr-functions.c"

#include <stdio.h>

int main(void) {
   for (int i = 0; i < 250; i++) {
      u16 x = i;
      u32 z = toBctNaive(x);
      u16 x_re = fromBctNaive(z);
      printf("%3d %05x %3d\n", x, z, x_re);
   }

   for (u16 x = 59049; x-- > (59049 -30); ) {
      u32 z = toBctNaive(x);
      u32 x_re = fromBctNaive(z);
      printf ("%5d %05x %5d\n", x, z, x_re);
   }
}
