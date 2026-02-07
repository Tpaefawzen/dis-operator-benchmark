#include "int.h"
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
} test_cases[50];

int main(void) {
   int ret = 0;

   for (isize i = 0; i < sizeof(test_cases)/sizeof(*test_cases); i++) {
      u16 x = (7 * ((u16)i % 7));
      u16 y = 7 * ((u16)i / 7);
      u16 z = opr_00_naive(x, y);
      test_cases[i].x = x, test_cases[i].y = y, test_cases[i].z = z;
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
