#include <stdlib.h>
#include "Hero.h"
#include "hero1_up.xpm"
#include "hero1_down.xpm"
#include "hero1_left.xpm"
#include "hero1_right.xpm"
#include "bullet.xpm"


Hero * create_hero(){

  Hero * hero_ptr = malloc(sizeof (Hero));
  xpm_image_t  hero_img;

  convert_xpm_img(hero1_xpm,&hero_img);
  hero_ptr->sprites[0] = hero_img;
  
  convert_xpm_img(hero1_left_xpm,&hero_img);
  hero_ptr->sprites[1] = hero_img;
  
  convert_xpm_img(hero1_right_xpm,&hero_img);
  hero_ptr->sprites[2] = hero_img;
  
  convert_xpm_img(hero1_down_xpm,&hero_img);
  hero_ptr->sprites[3] = hero_img;


  hero_ptr->x = HERO_START_POSITION_X;
  hero_ptr->y = HERO_START_POSITION_Y;
  hero_ptr->new_x = HERO_START_POSITION_X;
  hero_ptr->new_y = HERO_START_POSITION_Y;
  hero_ptr->x_direction = 0;
  hero_ptr->y_direction = 1;
  hero_ptr->hero_img = (hero_ptr->sprites[0]);
  hero_ptr->bullet = NULL;
  hero_ptr->direction = up;
  
  draw_xpm(hero_ptr->hero_img,hero_ptr->x,hero_ptr->y);
 
  
  return hero_ptr;
}


Bullet * shoot(Hero * hero){

  Bullet * bullet_ptr = malloc(sizeof (Bullet));

  xpm_image_t bullet_img;
  convert_xpm_img(bullet_xpm, &bullet_img);

  uint16_t bullet_x, bullet_y;

  get_bullet_coords(hero, &bullet_x, &bullet_y);

  bullet_ptr->sprite = create_sprite(bullet_img,bullet_x, bullet_y,5);

  get_bullet_direction(hero, &(bullet_ptr->x_direction), &(bullet_ptr->y_direction));

  draw_xpm(bullet_img, bullet_ptr->sprite->x, bullet_ptr->sprite->y);

  return bullet_ptr;
}


void hero_move_up(Hero * hero) {
  hero->new_y = hero->y - 10;
  hero->y_direction = -1;
  hero->x_direction = 0;
}

void hero_move_down(Hero * hero){
    hero->new_y  = hero->y + 10;
    hero->y_direction = 1;
    hero->x_direction = 0;
}

void hero_move_left(Hero * hero){
    hero->new_x = hero->x - 10;
    hero->x_direction = -1;
    hero->y_direction = 0;
}

void hero_move_right(Hero * hero){
    hero->new_x  = hero->x + 10;
    hero->x_direction = 1;
    hero->y_direction = 0;
}


void destroy_bullet(Bullet ** bullet) {

  if(*bullet == NULL){
    return;
  }
  free(*bullet);
  *bullet = NULL;
}

void update_xpm(Hero * hero){

    switch(hero->direction){
    case up:
      clean_xpm(hero->hero_img, hero->x, hero->y);

      hero->hero_img = (hero->sprites[0]);

      draw_xpm(hero->hero_img, hero->x, hero->y);
      break;
    case left:
      clean_xpm(hero->hero_img, hero->x, hero->y);

      hero->hero_img = (hero->sprites[1]);

      draw_xpm(hero->hero_img, hero->x, hero->y);
      break;
    case right:
      clean_xpm(hero->hero_img, hero->x, hero->y);

      hero->hero_img = (hero->sprites[2]);

      draw_xpm(hero->hero_img, hero->x, hero->y);
      break;
    case down:
      clean_xpm(hero->hero_img, hero->x, hero->y);

      hero->hero_img = (hero->sprites[3]);

      draw_xpm(hero->hero_img, hero->x, hero->y);
      break;
  }

}

void get_bullet_coords(Hero* hero, uint16_t * x, uint16_t * y){

  uint16_t hero_width = (hero->hero_img).width;
  uint16_t hero_height = (hero->hero_img).height;

  switch(hero->direction){
    case up:
      *x = hero->x + hero_width/2.3;
      *y = hero->y;
      break;
    case left:
      *x = hero->x;
      *y = hero->y + hero_height/3.25;
      break;
    case right:
      *x = hero->x + hero_width;
      *y = hero->y + hero_height/2.3;
      break;
    case down:
      *x = hero->x + hero_width/3.25;
      *y = hero->y + hero_height;
      break;
  }

  return;
}

void get_bullet_direction(Hero * hero, uint16_t * x, uint16_t * y){

  switch(hero->direction){
    case up:
      *x = 0;
      *y = -1;
      break;
    case left:
      *x = -1;
      *y = 0;
      break;
    case right:
      *x = 1;
      *y = 0;
      break;
    case down:
      *x = 0;
      *y = 1;
      break;
  }
}












