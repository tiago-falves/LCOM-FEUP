#include <stdlib.h>
#include "i8042.h"
#include "i8254.h"
#include "Hero.h"
#include "Zombie.h"
#include "mouse.h"
#include "background.xpm"



enum game_states {menu, level, learderboard, end};

typedef struct {

  Mouse * mouse;

  Hero * hero;

  Zombie * zombies[6];

  int nr_zombies;
  int nr_dead_zombies;


  //Mouse * mouse;

  //struct Zombie * zombies[6] = {zombie1,zombie2,zombie3,zombie4,zombie5,zombie6};

  enum game_states state;


  uint8_t x;
  xpm_image_t  background_xpm;

  vbe_mode_info_t mode_info;

} Terminix;

Terminix * create_terminix();
Terminix * get_current_terminix();

