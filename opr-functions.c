#include "int.h"

u16 opr_00_naive(u16 x, u16 y) {
   u16 ret = 0, weight = 1;
   for (; weight < 59049; weight *= 3) {
      ret += weight * ( ( (x/weight)%3 + 3 - (y/weight)%3 ) % 3 );
   }
   return ret;
}

u16 opr_00b_naiveRecursive(u16 x, u16 y) {
   if (!y) return x;
   return ((u16)(x%3) + 3 - (u16)(y%3)) % 3 + 3 * opr_00b_naiveRecursive(x/3, y/3);
}

u16 opr_01_lookup1by1(u16 x, u16 y) {
   static u16 opr_table[3][3] = {
      {0, 2, 1},
      {1, 0, 2},
      {2, 1, 0},
   };
   u16 ret = 0, weight = 1;
   for (; weight < 59049; weight *= 3) {
      ret += weight * opr_table[(x/weight)%3][(y/weight)%3];
   }
   return ret;
}

u16 opr_02_lookup2by2(u16 x, u16 y) {
   static u16 opr_table[9][9] = {
      {0, 2, 1, 6, 8, 7, 3, 5, 4},
      {1, 0, 2, 7, 6, 8, 4, 3, 5},
      {2, 1, 0, 8, 7, 6, 5, 4, 3},
      {3, 5, 4, 0, 2, 1, 6, 8, 7},
      {4, 3, 5, 1, 0, 2, 7, 6, 8},
      {5, 4, 3, 2, 1, 0, 8, 7, 6},
      {6, 8, 7, 3, 5, 4, 0, 2, 1},
      {7, 6, 8, 4, 3, 5, 1, 0, 2},
      {8, 7, 6, 5, 4, 3, 2, 1, 0},
   };
   u16 ret = 0, weight = 1;
   for (; weight < 59049; weight *= 9) {
      ret += weight * opr_table[(x/weight)%9][(y/weight)%9];
   }
   return ret;
}

static u32 toBctNaive(u16 x) {
   // to little endian but whatever
   u32 ret = 0;
   for (u16 weight = 1; weight < 59049; weight *= 3) {
      ret = (ret << 2) | (x % 3);
      x /= 3;
   }
   return ret;
}


static u16 fromBctNaive(u32 x) {
   // Note from little endian
   u16 ret = 0;
   for (u16 weight = 1; weight < 59049; weight *= 3) {
      ret *= 3;
      ret += x & 0x03;
      x >>= 2;
   }
   return ret;
}

u16 opr_03_toBctNaive(u16 x0, u16 y0) {
   u32 x = toBctNaive(x0);
   u32 y = toBctNaive(y0);

   u32 t = x ^ (y << 1);
   u32 z1_0 = t & (~(x<<1)) & (~y);
   u32 z1_1 = (~t) & (x<<1) & y;
   u32 z1 = z1_0 | z1_1;
   z1 &= 0xaaaaaaaa;

   t = x ^ (y >> 1);
   u32 z0_0 = t & (~(x >> 1)) & (~y);
   u32 z0_1 = (~t) & (x >> 1) & y;
   u32 z0 = z0_0 | z0_1;
   z0 &= 0x55555555;

   u32 z = z1 | z0;
   return fromBctNaive(z);
}
