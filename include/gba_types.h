#ifndef _GBA_TYPES_H_
#define _GBA_TYPES_H_

#ifdef __cplusplus
#include <cstdint>
extern "C" {
#else
#include <stdint.h>
#include <stdbool.h>
#endif

typedef int32_t fp_24_8_t;
typedef bool bool_t;
#define FP24_8_SHAMT 8
#define FP24_8_SCALE (1<<FP24_8_SHAMT)
#define FP24_8_SCALEF ((float)FP24_8_SCALE)
#define FP24_8_FRACTION_MASK (FP24_8_SCALE-1)

typedef struct coord {
  uint16_t x, y;
} Coord_t;
typedef struct fixed_coord {
  fp_24_8_t x, y;
} Fixed_Coord_t;

typedef enum rendertest_colorcodes {
  RC_EMPTY=0,
  RC_WHITE=1,
  RC_BLUE=2,
  RC_RED=3,
  RC_GREEN=4,
  RC_MATTE_BLACK=5
} Raycaster_ColorCodes;

extern const int raycaster_colortable[6];

#ifdef __cplusplus
}
#endif


#endif
