#ifndef _GBA_STDIO_H_
#define _GBA_STDIO_H_

#include "gba_types.h"
#ifdef __cplusplus
#include <cstdarg>
extern "C" {
#else
#include <stdarg.h>
#endif  /* C++ Name Mangler Guard */

#ifdef __cplusplus
bool_t Mode3_printf(uint32_t x, uint32_t y, uint16_t color, const char *fmt, ...);
bool_t Mode3_puts(const char *s, int x, int y, uint16_t color);
#else
bool_t Mode3_printf(uint32_t x, uint32_t y, uint16_t color, const char *restrict fmt, ...);
bool_t Mode3_puts(const char* restrict s, int x, int y, uint16_t color);
#endif  /* No restrict keyword in C++ */


bool_t Mode3_putchar(uint32_t x, uint32_t y, char c, uint16_t color);

#ifdef __cplusplus
}
#endif  /* C++ Name Mangler Guard */


#endif  /* _GBA_STDIO_H_ */
