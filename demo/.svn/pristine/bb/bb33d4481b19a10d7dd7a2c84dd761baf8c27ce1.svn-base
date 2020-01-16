
#include "Zombie.h"
#include "black_zombie.xpm"


Zombie * create_zombie(uint16_t x, uint16_t y){

  Zombie * zombie_ptr = malloc(sizeof (Zombie));

  xpm_image_t  zombie_img;

  //Temporario Ele estar com imagem do hero! Fazer GIMP

  convert_xpm_img(black_zombie_xpm,&zombie_img);

  zombie_ptr->sprite = create_sprite(zombie_img,x, y,1);
  
 
  zombie_ptr->sprite->img = zombie_img;
  
  draw_xpm(zombie_img,x,y);
 
  
  return zombie_ptr;
}


void destroy_zombie(Zombie ** zombie) {

  if(*zombie == NULL){
    return;
  }
  free(*zombie);
  *zombie = NULL;
}
