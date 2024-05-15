#include "gba_memmap.h"
#include "functions.h"
#include "gba_types.h"
#include "gba_memdef.h"
#include "gba_inlines.h"
#include "gba_util_macros.h"


#define FOV 0x1FFF
//#define FOV 0x2888
#define ANGLE_INCREMENTOR (FOV/SCREEN_WIDTH)

/// as fp_24_8_t. luminance factor is a fraction of (1.00=full luminance) that is
/// multiplied by ray magnitude and subbed out of 1 and multiplied by color of wall ray hit.
/// The fraction is 1/8th because 1 as int is 1<<8 as fp_24_8_t, so if int maths for one eighth is 1/8 = 1>>3
/// then fp_24_8_t maths for one eighth is (1<<8)>>3 = 1<<5
#define DEPTH_LUMINANCE_SCALE_FACTOR (1<<5)
#define FIXED_ONE (1<<8)

//#define BOARD_SCALE 1
#define DOF 10

IWRAM_CODE void render_raycast_view(const int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a) {
  Fixed_Coord_t r_xy;
  uint16_t *vram_ofs;
  fp_24_8_t dx, dy, dist, luminance;
  int ix, iy, top, btm, hit_wall = 0;
  uint16_t color, r,g,b;
  p_a -= (FOV>>1);
  for (int i = 0; i < SCREEN_WIDTH; ++i, p_a+=ANGLE_INCREMENTOR, hit_wall=0) {
    r_xy.x = p_xy->x;
    r_xy.y = p_xy->y;
    dx = lu_cos(p_a), dy = lu_sin(p_a);
    while (!hit_wall) {
      r_xy.x += dx>>2;
      r_xy.y += dy>>2;
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
      luminance = fp24_8_sub(FIXED_ONE, fp24_8_mul(dist, DEPTH_LUMINANCE_SCALE_FACTOR));
      r=((color&0x001F)*luminance)>>8;
      g=((color&0x03E0)*luminance)>>8;
      b = ((color&0x7C00)*luminance)>>8;
      color = (r&0x001F)|(g&0x03E0)|(b&0x7C00);
    } else {
      color = raycaster_colortable[RC_EMPTY];
    }




    top= (SCREEN_HEIGHT>>1)-((SCREEN_HEIGHT<<8)/(dist<<1));
    btm = SCREEN_HEIGHT-top;

    vram_ofs = &VRAM_BUFFER[i];
    
    for (int j = 0; j < top; ++j) {
      *vram_ofs = 0;
      vram_ofs+=SCREEN_WIDTH;
    }

    for (int j=top; j < btm; ++j) {
      *vram_ofs = color;
      vram_ofs+=SCREEN_WIDTH;
    }
    for (int j = btm; j < SCREEN_HEIGHT; ++j) {
      *vram_ofs = 0;
      vram_ofs += SCREEN_WIDTH;
    }
    
  }

}

IWRAM_CODE void render_topdown_view(int x, int y, const int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a) {
  uint16_t *const vram_ofs_orig = VRAM_BUFFER + y*SCREEN_WIDTH + x, *vram_ofs = vram_ofs_orig;
  const int *board_ofs = board;
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
  
  vram_ofs_orig[(player_y)*SCREEN_WIDTH+player_x] = 0x7CFD;
  player_x = fp24_8toi(p_xy->x + (lu_cos(p_a)<<1));
  player_y = fp24_8toi(p_xy->y + (lu_sin(p_a)<<1));
  vram_ofs_orig[(player_y)*SCREEN_WIDTH+player_x] = 0x7914;
}
