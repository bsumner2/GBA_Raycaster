#include "gba_inlines.h"
#include "gba_memdef.h"
#include "gba_memmap.h"
#include "functions.h"

#define TURN_RADIANS 196


static const int board[10][10] = {
  {4,4,1,1,1,1,1,1,2,2},
  {4,0,0,0,0,0,0,0,0,2},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,3,3,0,5,5,1},
  {1,0,0,0,3,3,0,5,0,1},
  {1,0,0,0,0,0,0,5,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {2,0,0,0,0,0,0,0,0,4},
  {2,2,1,1,1,1,1,1,4,4},
};

int main(void) {
  uint32_t player_theta = 0, whole_x=2, whole_y=2;
  Fixed_Coord_t player={ 2<<8, 2<<8}, prev=player;
  init_regs();
  for (;;) {
#ifdef RENDER_TOPDOWN
    vsync();
#endif
    vsync();
    if (!(REG_KEY&KEY_LEFT)) {
      player_theta-=TURN_RADIANS;
    } else if (!(REG_KEY&KEY_RIGHT)) {
      player_theta += TURN_RADIANS;
    }
    
    if (!(REG_KEY&KEY_UP)) {
      prev = player;
      player.x += lu_cos(player_theta)>>2;
      player.y += lu_sin(player_theta)>>2;
      whole_x = fp_24_8_whole(player.x);
      whole_y = fp_24_8_whole(player.y);
      if (board[whole_y][whole_x])
        player = prev;


      
    } else if (!(REG_KEY&KEY_DOWN)) {
      prev = player;
      player.x -= lu_cos(player_theta)>>2;
      player.y -= lu_sin(player_theta)>>2;
      whole_x = fp_24_8_whole(player.x);
      whole_y = fp_24_8_whole(player.y);
      if (board[whole_y][whole_x])
        player = prev;
    }
    render_raycast_view((const int*)board, 10, 10, &player, player_theta);
#ifdef RENDER_TOPDOWN
    render_topdown_view(10, 10, (const int*)board, 10, 10, &player, player_theta);
#endif
  }
  return 0; 
}
