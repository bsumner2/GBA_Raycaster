#ifndef _GBA_UTIL_MACROS_H_
#define _GBA_UTIL_MACROS_H_

#define IWRAM_CODE __attribute__((section(".iwram"), long_call))
#define EWRAM_CODE __attribute__((section(".ewram"), long_call))

#define IWRAM_DATA __attribute__((section(".iwram")))
#define EWRAM_DATA __attribute__((section(".ewram")))
#define EWRAM_BSS __attribute__((section(".sbss")))

#endif  /* _GBA_UTIL_MACROS_H_ */
