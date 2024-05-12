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



extern IWRAM_CODE void ISR_HANDLER_CB(void);
extern IWRAM_CODE void fast_memset32(void *dst, uint32_t fill_val, size_t word_count);
extern void vsync(void);
extern void init_regs(void);


#ifdef __cplusplus
}
#endif


#endif
