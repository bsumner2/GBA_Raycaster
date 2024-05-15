#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "verdana.h"

#include "gba_types.h"
#include "gba_memdef.h"
#include "gba_memmap.h"

// 1bpp 8x8 pixel buffer represents glyph
//typedef enum { } SymType;

static const uint8_t *verdana = (uint8_t*)verdana_GlyphData;

static const char ESC_FLAG_CHAR = '\x1b';
 
static int ipow(int base, uint16_t power) {
  if (power&1) return base*ipow(base*base, power>>1);
  else return power ? ipow(base*base, power>>1) : 1;
}

#define PARSE_ESC_CLR_ERROR_FLAG 0x8000

/*
 * @brief Use after encountering escape char, ASCII(27). Advance the pointer to
 * so that it's pointing to the char immediately following the esc char.
 * */
static uint16_t parse_color(char **buf_ptr) {
  char *str = *buf_ptr;
  uint16_t ret = 0;
  char tmp;
  int len = 0, i = 0;
  if (*str++ != '[')
    return PARSE_ESC_CLR_ERROR_FLAG;
  if (strncmp(str++, "0x", 2))
    return PARSE_ESC_CLR_ERROR_FLAG;
  while ((tmp = *++str) && tmp != ']') {
    if (!isxdigit(tmp) || ++len > 4)
      return PARSE_ESC_CLR_ERROR_FLAG;
  }
  if (!tmp || !len)
    return PARSE_ESC_CLR_ERROR_FLAG;
  if (!str[1]) {
    *buf_ptr = ++str;
    return 0;  // Color won't matter anyway if EOS reached.
  }
  for (tmp = tolower(*(str - 1 - (i = 0))); i < len;
      tmp = tolower(*(str - 1 - ++i))) {
    tmp = (tmp > '9' ? 10 + tmp - 'a' : tmp - '0');
    ret += tmp*ipow(16, i);
  }
  *buf_ptr = ++str;
  return 0x7FFF&ret;
}




/** 
 * @brief Print formatted string, fmt, with topleft starting point, (x,y) using
 * starting color, color.
 * @details Color flag should be formatted as ^ESC[<<color hexcode>>]. ^ESC
 * is ASCII(27), and can be represented as <<backslash>>x1b or <<backslash>>033
 * */
bool_t Mode3_printf(uint32_t x, uint32_t y, uint16_t color, const char *restrict fmt, 
    ...) {
  if (!fmt)
    return false;
  
  if (y + verdana_GlyphHeight > SCREEN_HEIGHT)
    return false;

  va_list args;
  size_t len;
  uint16_t *vbuf_row;
  uint8_t *cur_glyph;
  char *cursor;
  const int startx = x;
  int char_idx;
  uint16_t curr_width;
  uint16_t col = color;
  char tmp;
  uint8_t glyphrow;


  va_start(args, fmt);
  len = vsnprintf(NULL, 0, fmt, args);
  va_end(args);

  // Now that we have the length of what we need to print...
  char buf[len+1];
  va_start(args, fmt);
  vsnprintf(buf, len+1, fmt, args);
  if (buf[len]) buf[len] = '\0';
  va_end(args);

  cursor = buf;
  while ((tmp = *cursor++)) {
    if (tmp < ' ') {
      if (tmp=='\n') {
        y+=verdana_GlyphHeight;
        x = startx;
        if (y+verdana_GlyphHeight > SCREEN_HEIGHT)
          return false;
      } else if (tmp == ESC_FLAG_CHAR) {
        if ((col = parse_color(&cursor)) & PARSE_ESC_CLR_ERROR_FLAG) {
          return false;
        }
      } else {
        // TODO: Add support for other escape chars later.
      }
      continue;
    }
    char_idx = tmp - ' ';
    if (char_idx >= verdana_GlyphCount)
      continue;
    curr_width = verdana_GlyphWidths[char_idx];

    if (x+curr_width > SCREEN_WIDTH) {
      y+=verdana_GlyphHeight;
      x = startx;
      if (y+verdana_GlyphHeight > SCREEN_HEIGHT)
        return false;
    }

    cur_glyph = (uint8_t*)(verdana + verdana_CellSize*char_idx);
    for (int i = 0; i < verdana_GlyphHeight; ++i) {
      glyphrow = cur_glyph[i];
      vbuf_row = (y+i)*SCREEN_WIDTH + x + VRAM_BUFFER;
      for (int j = 0; j < curr_width; ++j) {
        if (!(glyphrow&(1<<j)))
          continue;
        else
          vbuf_row[j] = col;
      }
    }
    x += curr_width;
  }

  return true;
}








/** 
 * @brief Put string starting from topleft offset x, y with the color provided.
 * @return True if able to write everything, False if truncation was required.
 * */
bool_t Mode3_puts(const char* restrict s, int x, int y, uint16_t color) {
  const char *cur = s;
  uint16_t *vbuf_row;
  uint8_t *curr_glyph;
  const int startx=x;
  int char_idx;
  uint16_t curr_width;
  char c;
  uint8_t tmp;
  if (y + verdana_GlyphHeight > SCREEN_HEIGHT)
    return false;
  while ((c = *cur++)) {
    if (c < ' ') {
      if (c=='\n') {
        x = startx;
        y+=verdana_GlyphHeight;
      }
      continue;
    }
    char_idx = c-' ';
    if (char_idx >= verdana_GlyphCount)
      continue;
    curr_width = verdana_GlyphWidths[char_idx];


    if (x+curr_width > SCREEN_WIDTH) {
      y += verdana_GlyphHeight, x = startx;
      if (y + verdana_GlyphHeight > SCREEN_HEIGHT)
        return false;
    }
    curr_glyph = (uint8_t*)(verdana + verdana_CellSize*char_idx);
    for (int i = 0; i < verdana_CellHeight; ++i) {
      tmp = curr_glyph[i];
      vbuf_row = &VRAM_BUFFER[x + SCREEN_WIDTH*(i+y)];
      //vbuf_row = (y+i)*SCREEN_WIDTH + x + VRAM_BUFFER;
      for (int j = 0; j < curr_width; ++j) {
        if (!(tmp&(1<<j)))
          continue;
        else
          vbuf_row[j] = color;
      }
    }
    x += curr_width;
  }

  return true;
}


bool_t Mode3_putchar(uint32_t x, uint32_t y, char c, uint16_t color) {
  if (c <= ' ' ||c >= 127)
    return false;
  int idx = c - ' ';
  uint16_t width = verdana_GlyphWidths[idx];
  if (y+verdana_GlyphHeight>SCREEN_HEIGHT || x+width>SCREEN_WIDTH)
    return false;
  uint16_t *vbuf_row;
  uint8_t *glyph = (uint8_t*)(verdana + idx*verdana_CellSize), tmp;
  for(int i = 0; i < verdana_GlyphHeight; ++i) {
    vbuf_row = &VRAM_BUFFER[x + SCREEN_WIDTH*(i+y)];
    tmp = glyph[i];
    for (int j = 0; j < width; ++j) {
      if (!(tmp&(1<<j)))
        continue;
      else
        vbuf_row[j] = color;
    }
  }
  return true;

}

