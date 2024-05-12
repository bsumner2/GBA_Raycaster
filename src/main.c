#include "gba_inlines.h"
#include "gba_memdef.h"
#include "gba_memmap.h"
#include <stddef.h>
#include "functions.h"



static const int board[100] = {
  1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,
};

typedef struct coord {
  uint16_t x, y;
} Coord_t;



void draw_rect(int x, int y, int width, int height, uint16_t color) {
  uint32_t colorblock = color|(color<<16);
  uint16_t *vram_ofs = VRAM_BUFFER + y*240 + x;
  size_t wordct;
  if (x&1) {
    
    if (width&1) {
      wordct = width>>1;
      
      for (int i = 0; i < height; ++i) {
        *vram_ofs = color;
        fast_memset32(vram_ofs+1, colorblock, wordct);
        vram_ofs += 240;
      }
      return;
    }

    wordct = (--width)>>1;

    for (int i = 0; i < height; ++i) {
      *vram_ofs = color;

      fast_memset32(vram_ofs+1, colorblock, wordct);
      vram_ofs[width] = color;
      vram_ofs += 240;
    }

    return;
  }
  if (width&1) {
    wordct = (--width)>>1;
    for (int i = 0; i < height; ++i) {
      fast_memset32(vram_ofs, colorblock, wordct);
      vram_ofs[width] = color;
      vram_ofs+=240;
    }
    return;
  }

  wordct = width>>1;
  for (int i = 0; i < height; ++i) {
    fast_memset32(vram_ofs, colorblock, wordct);
    vram_ofs+=240;
  }
}



int main(void) {
  uint32_t player_theta = 0;
  int x, y, i, j;
  struct coord player = { 0, 0 };
  init_regs();
  for (;;) {
    vsync();
    if (!(REG_KEY&KEY_LEFT)) {
      player_theta+=64;
    } else if (!(REG_KEY&KEY_RIGHT)) {
      player_theta -= 64;
    }
    
    if (!(REG_KEY&KEY_UP)) {
      player.x += lulerp_cos(player_theta);
      player.y += lulerp_sin(player_theta);
    } else if (!(REG_KEY&KEY_DOWN)) {
      player.x -= lulerp_cos(player_theta);
      player.y -= lulerp_sin(player_theta);
    }

    for (i = 0; i < 10; ++i) {
      x = 40;
      y = 16*i;
      for (j = 0; j < 10; ++j) {
        if (board[(i<<1)+(i<<3) + j])
          draw_rect(x, y, 16, 16, 0x7FFF);
        x += 16;
      }
    }
    draw_rect(player.x>>8, player.y>>8, 4,4, 0x7C00);
  }

  return 0; 
}
