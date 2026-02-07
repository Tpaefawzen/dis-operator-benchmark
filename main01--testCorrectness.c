#include "int.h"
#include "Xorshift32.h"
#include "Random.h"
#include "opr-functions.h"

typedef struct FnAndName FnAndName;
static struct FnAndName {
   u16 (*oprFn)(u16, u16);
   u8 *name;
   isize name_len;
} functions[] ={
#define a(Fn) { (u16(*)(u16,u16))Fn, (u8*)#Fn, sizeof(#Fn)-1, }
#include "listOfOprFunctions.h"
#undef a
};

#include <stdio.h>

static struct TestCase {
   u16 x, y, z;
} test_cases[10000];

int main(void) {
   int ret = 0;
   Xorshift32 x32 = Xorshift32_init(0x0491FE95);
   std_Random rng = Xorshift32_random(&x32);

   for (isize i = sizeof(test_cases)/sizeof(*test_cases); i--; ) {
      test_cases[i].x = std_Random_uintLessThanBiased(&rng, u16, 59049);
      test_cases[i].y = std_Random_uintLessThanBiased(&rng, u16, 59049);
      test_cases[i].z = opr_00_naive(test_cases[i].x, test_cases[i].y);
   }

   for (FnAndName *iFn = functions, *endFn = &functions[sizeof(functions)/sizeof(*functions)]; iFn < endFn; iFn++) {
      for (isize i = sizeof(test_cases)/sizeof(*test_cases); i--; ) {
         u16 x = test_cases[i].x, y = test_cases[i].y, z = test_cases[i].z;
         u16 my_z = iFn->oprFn(x, y);
	 if (z != my_z) {
	    printf("%s: failed #%zd: %d - %d: expected: %d, got: %d\n",
		   iFn->name, i, x, y, z, my_z);
	    if( ++ret >= 20 ) return ret;
	 }
      }
   }

   return ret;
}
