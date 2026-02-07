#include <stdio.h>
#include <inttypes.h>

#include "int.h"
#include "getNanosecond.h"

int main(void) {
   u64 my_ns = getNanosecond();
   printf("%" PRId64 "\n", my_ns);
}
