#include "gba_memmap.h"
#include "functions.h"

#ifndef RENDER_IN_THUMB
#include "gba_types.h"
#include "gba_memdef.h"
#include "gba_inlines.h"
#include "gba_util_macros.h"
#define FOV 0x0FFF
#define ANGLE_INCREMENTOR (FOV/SCREEN_WIDTH)

//#define BOARD_SCALE 1
#define DOF 10
/*
#define DEBUG_KEY_BREAK(waitkey, text_x, text_y, text_clr, text_fmt, ...) \
  do {                                                                     \
    fast_memset32(VRAM_BUFFER, 0, 19200);                                   \
    Mode3_printf(text_x, text_y, text_clr, text_fmt, __VA_ARGS__);           \
    while (REG_KEY&waitkey) continue;                                         \
    while (!(REG_KEY&waitkey)) continue;                                       \
  } while(false)
*/

uint16_t render_buffer[240] = {0};
IWRAM_CODE uint16_t *get_render_buffer(int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a) {
  Fixed_Coord_t r_xy;
  uint16_t *vram_ofs;
  fp_24_8_t dx, dy, dist;
  int ix, iy, tmp, hit_wall = 0;
  uint16_t color;
  p_a -= (FOV>>1);
  for (int i = 0; i < SCREEN_WIDTH; ++i, p_a+=ANGLE_INCREMENTOR, hit_wall=0) {
    r_xy.x = p_xy->x;
    r_xy.y = p_xy->y;
    dx = lu_cos(p_a), dy = lu_sin(p_a);
    while (!hit_wall) {
      r_xy.x += dx>>1;
      r_xy.y += dy>>1;
      ix = fp24_8toi(r_xy.x);
      iy = fp24_8toi(r_xy.y);
      if (ix < 0 || iy < 0 || ix >= board_width || iy >= board_height) {
        dist = (10<<8);
        break;
      }
      hit_wall = board[iy*board_width+ix];
    }
    if (hit_wall) {
      r_xy.x -= p_xy->x;
      r_xy.y -= p_xy->y;
      dist = gba_sqrt((r_xy.x*r_xy.x)+(r_xy.y*r_xy.y));
      color = raycaster_colortable[hit_wall];
    } else {
      color = raycaster_colortable[RC_WHITE];
    }


    render_buffer[i] =tmp= (SCREEN_HEIGHT>>1)-((SCREEN_HEIGHT<<8)/(dist<<1));


    vram_ofs = &VRAM_BUFFER[i];
    
    for (int j = 0; j < tmp; ++j) {
      *vram_ofs = 0;
      vram_ofs+=SCREEN_WIDTH;
    }

    tmp = SCREEN_HEIGHT-tmp;
    for (int j=render_buffer[i]; j < tmp; ++j) {
      *vram_ofs = color;
      vram_ofs+=SCREEN_WIDTH;
    }
    for (int j = tmp; j < SCREEN_HEIGHT; ++j) {
      *vram_ofs = 0;
      vram_ofs += SCREEN_WIDTH;
    }
    
  }

  return render_buffer;
}

IWRAM_CODE void render_topdown(int x, int y, int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a) {
  const uint16_t *const vram_ofs_orig = VRAM_BUFFER + y*SCREEN_WIDTH + x;
  uint16_t *vram_ofs = (uint16_t*)vram_ofs_orig;
  int *board_ofs = board;
  int player_x, player_y;
  for (int i = 0; i < board_height; ++i) {
    for (int j = 0; j < board_width; ++j) {
      vram_ofs[j] = raycaster_colortable[board_ofs[j]];
    }
    vram_ofs += SCREEN_WIDTH;
    board_ofs += board_width;
  }
  player_x = fp24_8toi(p_xy->x);
  player_y = fp24_8toi(p_xy->y);
  
  vram_ofs = (uint16_t*)vram_ofs_orig;
  vram_ofs[(player_y)*SCREEN_WIDTH+player_x] = 0x7CFD;
  player_x = fp24_8toi(p_xy->x + (lu_cos(p_a)<<1));
  player_y = fp24_8toi(p_xy->y + (lu_sin(p_a)<<1));
  vram_ofs[(player_y)*SCREEN_WIDTH+player_x] = 0x7914; 
}


#endif
