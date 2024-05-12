#include "gba_memmap.h"
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
