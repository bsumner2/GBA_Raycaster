#ifndef _GBA_INLINES_H_
#define _GBA_INLINES_H_
#include "gba_types.h"
#include "gba_sine_lut.h"
#ifdef __cplusplus
extern "C" {
#endif


#define INLINE static inline

INLINE fp_24_8_t itofp24_8(int32_t x) { return x<<FP24_8_SHAMT; }
INLINE fp_24_8_t ftofp24_8(float f) { return (fp_24_8_t)(f*FP24_8_SCALEF); }
INLINE uint32_t fp_24_8_whole(fp_24_8_t fp) { return fp>>FP24_8_SHAMT; }
INLINE uint32_t fp_24_8_fraction(fp_24_8_t fp) { return fp&FP24_8_FRACTION_MASK; }
INLINE int32_t fp24_8toi(fp_24_8_t fp) { return fp/FP24_8_SCALE; }
INLINE float fp24_8tof(fp_24_8_t fp) { return fp/FP24_8_SCALEF; }

INLINE fp_24_8_t fp24_8_add(fp_24_8_t a, fp_24_8_t b) { return a+b; }
INLINE fp_24_8_t fp24_8_sub(fp_24_8_t minnuend, fp_24_8_t subtrahend) { return minnuend - subtrahend; }
INLINE fp_24_8_t fp24_8_mul(fp_24_8_t a, fp_24_8_t b) { return (a*b)>>FP24_8_SHAMT; }
INLINE fp_24_8_t fp24_8_div(fp_24_8_t dividend, fp_24_8_t divisor) { return ((divisor)*FP24_8_SCALE)/(divisor); }

INLINE fp_24_8_t lu_sin(uint32_t theta) { return sin_lut[(theta>>7)&511]; }
INLINE fp_24_8_t lu_cos(uint32_t theta) { return sin_lut[((theta>>7)+128)&511]; }


INLINE int16_t lu_lerp16(const int16_t lut[], int x, const int shift)
{
    int xa, ya, yb;
    xa=x>>shift;
    ya= lut[xa]; yb= lut[xa+1];
    return ya + ((yb-ya)*(x-(xa<<shift))>>shift);
}

INLINE fp_24_8_t lulerp_sin(uint32_t theta) {
  return lu_lerp16(sin_lut, theta, 7);
}

INLINE fp_24_8_t lulerp_cos(uint32_t theta) { return lu_lerp16(sin_lut, 0xFFFF&(theta+16384), 7); }

#ifdef __cplusplus
}
#endif

#endif  /* _GBA_INLINES_H_ */
