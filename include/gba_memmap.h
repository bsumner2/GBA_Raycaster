#ifndef _GBA_MEMMAP_H_
#define _GBA_MEMMAP_H_

#include <gba_types.h>


#ifdef __cplusplus
extern "C" {
#endif

#define MEM_IO 0x04000000
#define REG_BASE MEM_IO

#define MEM_VRAM 0x06000000

#define REG_DISPLAY_CNT *((volatile uint32_t*) (MEM_IO))
#define REG_DISPLAY_STAT *((volatile uint16_t*) (MEM_IO + 0x0004))

#define REG_KEY *((volatile uint16_t*) (MEM_IO+0x0130))
#define VRAM_BUFFER ((uint16_t*) (MEM_VRAM))

#define REG_IE *((volatile uint16_t*)   (REG_BASE+0x0200))
#define REG_IF *((volatile uint16_t*)   (REG_BASE+0x0202))
#define REG_IEIF *((volatile uint32_t*) (REG_BASE+0x0200))
#define REG_IME *((volatile uint16_t*)  (REG_BASE+0x0208))

typedef void (*ISR_CB)(void);

#define REG_ISR_MAIN *((ISR_CB*) (REG_BASE-4))
#define REG_IFBIOS *((volatile uint16_t*) (REG_BASE-8))








#ifdef __cplusplus
}
#endif


#endif  /* _GBA_MEMMAP_H_ */
