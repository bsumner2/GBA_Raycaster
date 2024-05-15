#include "gba_memmap.h"
#include "gba_memdef.h"
#include "functions.h"
void init_regs(void) {
  // set video mode to 3 and bg 2 enabled
  REG_DISPLAY_CNT=0x0403;
  // switch master IRQ enable off while we set up interrupt handler for vsync
  REG_IME = 0;
  REG_IE = 1;
  REG_DISPLAY_STAT = 8;
  REG_ISR_MAIN = ISR_HANDLER_CB;
  // ...then switch it back on
  REG_IME = 1;
}


#ifdef RENDER_IN_THUMB
#include "gba_inlines.h"
#include "gba_types.h"
#define FOV 0x0FFF
#define ANGLE_INCREMENTOR (FOV/SCREEN_WIDTH)

//#define BOARD_SCALE 1
#define DOF 10

uint16_t render_buffer[240] = {0};

uint16_t *get_render_buffer(int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a) {
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


    render_buffer[i] =tmp= (SCREEN_HEIGHT>>1)-((SCREEN_HEIGHT<<8)/(dist>>1));


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

void render_topdown(int x, int y, int *board, int board_width, int board_height, const Fixed_Coord_t *p_xy, uint32_t p_a) {
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
  vram_ofs[(y+player_y)*board_width+x+player_x] = 0x7CFD;
  player_x = fp24_8toi(p_xy->x + (lu_cos(p_a)<<1));
  player_y = fp24_8toi(p_xy->y + (lu_sin(p_a)<<1));
  vram_ofs[(y+player_y)*board_width+x+player_x] = 0x7914; 
}



#endif

void draw_rect(int x, int y, int width, int height, uint16_t color) {
  uint32_t colorblock = color|(color<<16);
  uint16_t *vram_ofs = VRAM_BUFFER + y*SCREEN_WIDTH + x;
  size_t wordct;
  if (x&1) {
    
    if (width&1) {
      wordct = width>>1;
      
      for (int i = 0; i < height; ++i) {
        *vram_ofs = color;
        fast_memset32(vram_ofs+1, colorblock, wordct);
        vram_ofs += SCREEN_WIDTH;
      }
      return;
    }

    wordct = (--width)>>1;

    for (int i = 0; i < height; ++i) {
      *vram_ofs = color;

      fast_memset32(vram_ofs+1, colorblock, wordct);
      vram_ofs[width] = color;
      vram_ofs += SCREEN_WIDTH;
    }

    return;
  }
  if (width&1) {
    wordct = (--width)>>1;
    for (int i = 0; i < height; ++i) {
      fast_memset32(vram_ofs, colorblock, wordct);
      vram_ofs[width] = color;
      vram_ofs+=SCREEN_WIDTH;
    }
    return;
  }

  wordct = width>>1;
  for (int i = 0; i < height; ++i) {
    fast_memset32(vram_ofs, colorblock, wordct);
    vram_ofs+=SCREEN_WIDTH;
  }
}


