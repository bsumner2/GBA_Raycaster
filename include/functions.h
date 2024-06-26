#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#include "gba_types.h"
#include "gba_util_macros.h"

#ifdef __cplusplus
#include <cstddef>
extern "C" {
#else
#include <stddef.h>
#endif



IWRAM_CODE void ISR_HANDLER_CB(void);
IWRAM_CODE void fast_memset32(void *dst, uint32_t fill_val, size_t word_count);

IWRAM_CODE void render_raycast_view(const int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a);
IWRAM_CODE void render_topdown_view(int x, int y, const int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a);

void vsync(void);
void init_regs(void);
uint16_t gba_sqrt(uint32_t x);
void draw_rect(int x, int y, int width, int height, uint16_t color);

#ifdef __cplusplus
}
#endif


#endif
