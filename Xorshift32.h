#pragma once

typedef struct Xorshift32 {
   u32 a;
   u32 mask;
} Xorshift32;

#define Xorshift32_init(seed) (Xorshift32){ (u32)(seed), (u32)(0xff)<<24, }

static void Xorshift32_next4(void *self0) {
   Xorshift32 *self = self0;
   u32 x = self->a;
   x ^= x << 13;
   x ^= x >> 17;
   x ^= x << 5;
   self->a = x;
}

static u8 Xorshift32_nextByte(void *self0) {
   Xorshift32 *self = self0;
   u32 ret0 = (self->a) & (self->mask);
   u8 ret = ( ret0 | (ret0 >> 8) | (ret0 >> 16) | (ret0 >> 24) ) & 0xff;
   if (self->mask >>= 8) return ret;

   Xorshift32_next4(self);
   self->mask = (u32)0xff << 24;
   return ret;
}

static void Xorshift32_fill(void *self0, u8 *buf, isize len) {
   Xorshift32 *self = self0;
   for (isize i = 0; i < len; i++) {
      buf[i] = Xorshift32_nextByte(self);
   }
   return;
}

#define Xorshift32_random(self) (std_Random){ self, &Xorshift32_fill }
