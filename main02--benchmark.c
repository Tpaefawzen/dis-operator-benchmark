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
#include <inttypes.h>

#include "getNanoseconds.h"

int main(void) {
   enum { rng_seed = 0x0491FE95, };
   int ret = 0;

   static isize n_oprs[] = { 1e2, 1e4, 1e6, 1e8, };

   for (FnAndName *iFn = functions, *endFn = &functions[sizeof(functions)/sizeof(*functions)]; iFn < endFn; iFn++) {
      Xorshift32 x32 = Xorshift32_init(rng_seed);
      std_Random rng = Xorshift32_random(&x32);

      for (isize i_n_oprs = 0; i_n_oprs < sizeof(n_oprs)/sizeof(*n_oprs); i_n_oprs++) {
	 u16 (*oprFn)(u16, u16) = iFn->oprFn;
	 u64 ns_begin = getNanosecond();
	 for (isize oprs = n_oprs[i_n_oprs]; oprs--; ) {
	    u16 x = std_Random_uintLessThanBiased(&rng, u16, 59049);
	    u16 y = std_Random_uintLessThanBiased(&rng, u16, 59049);
	    (void) oprFn(x, y);
	 }
	 u64 ns_end = getNanosecond();
	 u64 ns_diff = ns_end - ns_begin;
	 printf("%s: %zu tests, %" PRId64 " ns\n",
		iFn->name, n_oprs[i_n_oprs], ns_diff);
      }
   }

   return ret;
}
