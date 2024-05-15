#include <stddef.h>
#include "gba_inlines.h"
#include "gba_memdef.h"
#include "gba_memmap.h"
#include "functions.h"



static const int board[100] = {
  4,4,1,1,1,1,1,1,2,2,
  4,0,0,0,0,0,0,0,0,2,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,1,
  1,0,0,0,3,3,0,5,5,1,
  1,0,0,0,3,3,0,5,0,1,
  1,0,0,0,0,0,0,5,0,1,
  1,0,0,0,0,0,0,0,0,1,
  2,0,0,0,0,0,0,0,0,4,
  2,2,1,1,1,1,1,1,4,4,
};

int main(void) {
  uint32_t player_theta = 0, prevtheta;
  int x, y, i, j;
  Fixed_Coord_t player={ 2<<8, 2<<8}, prev=player;
  bool_t moved =true, anglechange=true;
  init_regs();
  for (;;) {
#ifdef RENDER_TOPDOWN
    vsync();
//    vsync();
#endif
    vsync();
#define TURN_RADIANS 96
    if (!(REG_KEY&KEY_LEFT)) {
      anglechange = true;
      prevtheta = player_theta;
      player_theta-=TURN_RADIANS;
    } else if (!(REG_KEY&KEY_RIGHT)) {
      anglechange=true;
      prevtheta = player_theta;
      player_theta += TURN_RADIANS;
    }
    
    if (!(REG_KEY&KEY_UP)) {
      prev = player;
      player.x += lu_cos(player_theta)>>2;
      player.y += lu_sin(player_theta)>>2;
      moved = true;
    } else if (!(REG_KEY&KEY_DOWN)) {
      prev = player;
      player.x -= lu_cos(player_theta)>>2;
      player.y -= lu_sin(player_theta)>>2;
      moved = true;
    }
    get_render_buffer((int*)board, 10, 10, &player, player_theta);
#ifdef RENDER_TOPDOWN
    render_topdown(10, 10, (int*)board, 10, 10, &player, player_theta);
#endif
  }
  return 0; 
}
