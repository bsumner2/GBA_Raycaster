#ifndef _GBA_TYPES_H_
#define _GBA_TYPES_H_

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#endif

typedef int32_t fp_24_8_t;
#define FP24_8_SHAMT 8
#define FP24_8_SCALE (1<<FP24_8_SHAMT)
#define FP24_8_SCALEF ((float)FP24_8_SCALE)
#define FP24_8_FRACTION_MASK (1-FP24_8_SCALE)


#ifdef __cplusplus
}
#endif


#endif
