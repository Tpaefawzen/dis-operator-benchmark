#include "int.h"
#include "Random.h"

typedef struct root_SequentialPrng {
   u8 next_val;
} root_SequencialPrng;

#define root_SequencialPrng_init() { 0 }

static void root_SequencialPrng_fill(root_SequencialPrng *self, u8 *buf, isize len) {
   for (; len >= 0; )
      buf[len] = self->next_val;
   self->next_val += 1;
}

static std_Random root_SequencialPrng_random(root_SequencialPrng *r) {
   return std_Random_init(r, (void(*)(void*,u8*,isize))&root_SequencialPrng_fill);
}

int main(void) {
   std_Random r = std_Random_init();
}
