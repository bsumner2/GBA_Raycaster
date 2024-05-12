#include "gba_memmap.h"
#include "gba_util_macros.h"
IWRAM_CODE void ISR_HANDLER_CB(void) {
  uint32_t ieif = REG_IEIF;
  ieif &= (ieif>>16);
  // ACK interrupt and exit
  REG_IF = ieif;
  REG_IFBIOS |= ieif; 
}
