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


