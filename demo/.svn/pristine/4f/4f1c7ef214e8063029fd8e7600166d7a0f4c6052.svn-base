#include "Terminix.h"


static Terminix * terminix = NULL;


Terminix * get_current_terminix() {
  return terminix;
}


Terminix * create_terminix(){

  if(terminix != NULL){
    return terminix;
  }

  terminix = malloc(sizeof (Terminix));
  
  xpm_image_t background_img;
  convert_xpm_img(background_xpm,&background_img);
  draw_xpm(background_img,0,0);
  terminix->background_xpm = background_img;

  Hero * hero = create_hero();

  Zombie * zombie1 = create_zombie(200,100);
  Zombie * zombie2 = create_zombie(400,700);


  terminix->nr_zombies = 2;
  terminix->nr_dead_zombies = 0;
  
  vbe_mode_info_t current_mode_info;

  get_vbe_mode_info(RGB_VIDEO_MODE, &current_mode_info);

  terminix->mode_info = current_mode_info;

  Mouse * mouse = create_mouse();

  terminix->hero = hero;
  terminix->zombies[0] = zombie1;
  terminix->zombies[1] = zombie2;
  // terminix->zombies[2] = zombie3;
  terminix->mouse = mouse;

  terminix->state = level;

  return terminix;
}


