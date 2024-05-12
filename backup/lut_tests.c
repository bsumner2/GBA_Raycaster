#include "gba_memmap.h"
#include "gba_types.h"
#include "gba_inlines.h"
#include "functions.h"

#define BLUE  0x7C00
#define GREEN 0x03E0



#define RANGE 0x10000

int main(void) {
  int x=0, y;
  init_regs();
  for (int i = 0; x < 240 && i < RANGE; i+=256) {
#if defined(GRAPH_SINE)
    vsync();
    y = 79 - ((lu_sin(i)*80)>>8);
    VRAM_BUFFER[x + y*240] = BLUE;

    vsync();
    y = 79 - ((lu_lerp16(sin_lut, i, 7)*80)>>8);
    VRAM_BUFFER[x++ + y*240] = GREEN;
#elif defined(GRAPH_COS)
    vsync();
    y = 79 - ((lu_cos(i)*80)>>8);
    VRAM_BUFFER[x + y*240] = BLUE;
    
    vsync();
    y = 79 - ((lu_lerp16(sin_lut, 0xFFFF&(i+16384), 7)*80)>>8);
    VRAM_BUFFER[x++ + y*240] = GREEN;
#else
    vsync();
    y = 79 - ((lulerp_sin(i)*5)>>4);  // midpt_ycoord - ((sin(i)*80)/256) = mid-((sin(i)*5*16)/(16*16)) = mid-((sin(i)*5)>>4)
    VRAM_BUFFER[x + y*240] = BLUE;
  
    vsync();
    y = 79 - ((lulerp_cos(i)*5)>>4);
    VRAM_BUFFER[x++ + y*240] = GREEN;
#endif
  }

  for (;;)
    vsync();
}
