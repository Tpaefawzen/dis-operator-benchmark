#pragma once

typedef struct std_Random {
   void *ctx;
   void (*fillFn)(void*, u8*, isize);
} std_Random;

static std_Random std_Random_init(void *ptr, void (*fillFn)(void*, u8*, isize)) {
   return (std_Random){ ptr, fillFn, };
}

#define std_Random_int(r, T) (std_Random_int_##T(r))

static u16 std_Random_int_u16(void *r0) {
   std_Random *r = r0;
   enum { bits = 16, };
   enum { ceil_bytes = (u16)bits / 8, };
   u8 read_bytes[2];
   r->fillFn(r->ctx, read_bytes, sizeof(read_bytes)/sizeof(*read_bytes));
   return read_bytes[0] | (read_bytes[1] << 8);
}

#define std_Random_uintLessThanBiased(r,T, less_than) \
   (std_Random_limitRangeBiased_##T(std_Random_int(r, T), less_than))

static u16 std_Random_limitRangeBiased_u16(u16 x, u16 less_than) {
   u32 m = x * less_than;
   return m >> 16;
}

