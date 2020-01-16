
#include "Game.h"
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include <time.h>
#include <stdint.h>

int subscribe_periphericals(int *timer_irq, int *mouse_irq, int *keyboard_irq) {
  uint8_t bit_no;

  if (mouse_subscribe(&bit_no)) {
    printf("Error subscribing mouse.\n");
    return 1;
  }

  *mouse_irq = BIT(bit_no);

  // Enable Mouse Data Reporting
  mouse_disable_interrupts();
  mouse_send_command(MOUSE_EN_DATA_REPORTING);
  mouse_enable_interrupts();
  if (keyboard_subscribe(&bit_no)) {
    printf("Error subscribing keyboard.\n");
    return 1;
  }
  *keyboard_irq = BIT(bit_no);
  // Subscribing to the timer
  if (timer_subscribe_int(&bit_no)) {
    printf("Error subscribing timer.\n");
    return 1;
  }
  *timer_irq = BIT(bit_no);
  return 0;
}

int unsubscribe_periphericals() {
  // Disables Mouse Data Reporting
  mouse_disable_interrupts();
  mouse_send_command(MOUSE_DIS_DATA_REPORTING);
  mouse_enable_interrupts();
  // Unsubscribing to the Mouse
  if (mouse_unsubscribe()) {
    printf("Error unsubscribing mouse.\n");
  }
  if (keyboard_unsubscribe()) {
    printf("Error unsubscribing keyboard.\n");
    return 1;
  }
  if (timer_unsubscribe_int()) {
    printf("Error unsubscribing timer.\n");
    return 1;
  }
  // Checks if the OBF is full and if it is, it cleans it
  if (mouse_flush_OBF()) {
    printf("Error flushing OBF byte \n");
    return 1;
  }
  return 0;
}

void render() {
  update_positions();
  update_buffer();
}

void update_positions() {

  // Update hero position
  Terminix *terminix = get_current_terminix();

  update_hero_position(terminix);
  
  
  update_zombies_position(terminix->zombies, terminix->nr_zombies);

  update_mouse_img(terminix->mouse);


  if (terminix->hero->bullet != NULL){
    update_bullets_position(terminix->hero->bullet);
  }

  if(terminix->nr_dead_zombies == terminix->nr_zombies){
    terminix->state = end;
  }
  
}

void update_hero_position(Terminix * terminix) {

  clean_xpm(terminix->hero->hero_img, terminix->hero->x, terminix->hero->y);
  
  // Update Hero position
  terminix->hero->x = terminix->hero->new_x;

  if(terminix->hero->x < 10){
    terminix->hero->x = 10;
  }
  else if(terminix->hero->x > (terminix->mode_info).XResolution - (terminix->hero->hero_img).width){
    terminix->hero->x = (terminix->mode_info).XResolution - (terminix->hero->hero_img).width;
  }

  terminix->hero->y = terminix->hero->new_y;
  if(terminix->hero->y < 10){
    terminix->hero->y = 10;
  }
  else if(terminix->hero->y > (terminix->mode_info).YResolution - (terminix->hero->hero_img).height){
    terminix->hero->y = (terminix->mode_info).YResolution - (terminix->hero->hero_img).height;
  }

  
  check_hero_direction(terminix->hero, terminix->mouse);

  update_xpm(terminix->hero);

  draw_xpm(terminix->hero->hero_img, terminix->hero->x, terminix->hero->y);

  if(check_heroe_collisions(terminix->hero, terminix)){
    terminix->state = end;
    printf("You died");
  }
  
}

void update_mouse_img(Mouse * mouse) {
  clean_xpm(mouse->mouse_img, mouse->x, mouse->y);

  // Update mouse position
  mouse->x = mouse->new_x;
  mouse->y = mouse->new_y;

  draw_xpm(mouse->mouse_img, mouse->new_x, mouse->new_y);
}

void update_bullets_position(Bullet * bullet) {

  if(bullet == NULL){
    return;
  }

  Terminix * terminix = get_current_terminix();

  clean_xpm(bullet->sprite->img, bullet->sprite->x, bullet->sprite->y);
  // Update mouse position
  

  bullet->sprite->x += (bullet->x_direction) * (bullet->sprite->speed);
  bullet->sprite->y += (bullet->y_direction) * (bullet->sprite->speed);

  if(check_bullet_colisions(bullet, terminix)){
    destroy_bullet(&bullet);
    terminix->hero->bullet = NULL;
    return;
  }

  if((bullet->sprite->y < 0) || (bullet->sprite->x < 0) || (bullet->sprite->x > (terminix->mode_info).XResolution - (bullet->sprite->img).width) || (bullet->sprite->y > (terminix->mode_info).YResolution - (bullet->sprite->img).height)){
     destroy_bullet(&bullet);
     terminix->hero->bullet = NULL;

  } else
  {
    draw_xpm(bullet->sprite->img, bullet->sprite->x, bullet->sprite->y);
  }
  
}


void update_zombie_position(Zombie * zombie) {

  Terminix * terminix = get_current_terminix();

  clean_xpm(zombie->sprite->img, zombie->sprite->x, zombie->sprite->y);
  // Update mouse position

  if (terminix->hero->x > zombie->sprite->x){
    zombie->sprite->x += (zombie->sprite->speed);
  }
  if (terminix->hero->x < zombie->sprite->x){
    zombie->sprite->x -= (zombie->sprite->speed);
  }
  if (terminix->hero->y > zombie->sprite->y){
    zombie->sprite->y += (zombie->sprite->speed);
  }
  if (terminix->hero->y < zombie->sprite->y){
    zombie->sprite->y -= (zombie->sprite->speed);
  }

  
  if(zombie->sprite->y < 0){
    zombie->sprite->y = 3;
  }
  if(zombie->sprite->x < 0){
    zombie->sprite->x = 3;
  }
  if(zombie->sprite->x > (terminix->mode_info).XResolution){
    zombie->sprite->x = (terminix->mode_info).XResolution- (zombie->sprite->img.width) -3;
  }
  if( zombie->sprite->y > ((terminix->mode_info).YResolution - (zombie->sprite->img).height)){
    zombie->sprite->y = ((terminix->mode_info).YResolution - (zombie->sprite->img).height) -3;
  }

  draw_xpm(zombie->sprite->img, zombie->sprite->x, zombie->sprite->y);
  
  
}

void update_zombies_position(Zombie * zombies[], int n){
  
  for(int i = 0; i < n; i++){

    if(zombies[i] == NULL){
      continue;
    }
    update_zombie_position(zombies[i]);
  }
}

bool check_bullet_colisions(Bullet * bullet, Terminix * terminix){

  uint16_t x_image, y_image, image_width, image_height, bullet_width, bullet_height, bullet_x, bullet_y;

  bullet_width = bullet->sprite->x + (bullet->sprite->img).width;
  bullet_height = bullet->sprite->y + (bullet->sprite->img).height;
  bullet_x = bullet->sprite->x;
  bullet_y = bullet->sprite->y;

  bool colision_found = false;

  for(int i = 0; i < terminix->nr_zombies; i++){

    if(terminix->zombies[i] == NULL){
      continue;
    }

    x_image = terminix->zombies[i]->sprite->x;
    y_image = terminix->zombies[i]->sprite->y;
    image_width = x_image + (terminix->zombies[i]->sprite->img).width;
    image_height = y_image + (terminix->zombies[i]->sprite->img).height;

    
    if((bullet_x >= x_image && bullet_x <= image_width) || ((bullet_width >= x_image) && (bullet_width <= image_width))){

      if((y_image <= bullet_y && bullet_y <= image_height) || ((y_image <= bullet_height) && bullet_height <= image_height)){
        colision_found = true;
      }
      
    }

    if(colision_found){
      clean_xpm(terminix->zombies[i]->sprite->img, terminix->zombies[i]->sprite->x, terminix->zombies[i]->sprite->y);
      destroy_zombie(&(terminix->zombies[i]));
      terminix->zombies[i] = NULL;
      terminix->nr_dead_zombies++;
      return true;
    }
  }

  return false;
}

bool check_heroe_collisions(Hero * hero, Terminix * terminix){
  uint16_t x_image, y_image, image_width, image_height, hero_width, hero_height, hero_x, hero_y;

  hero_width = hero->x + (hero->hero_img).width;
  hero_height = hero->y + (hero->hero_img).height;
  hero_x = hero->x;
  hero_y = hero->y;

  for(int i = 0; i < terminix->nr_zombies; i++){

    if(terminix->zombies[i] == NULL){
      continue;
    }

    x_image = terminix->zombies[i]->sprite->x;
    y_image = terminix->zombies[i]->sprite->y;
    image_width = x_image + (terminix->zombies[i]->sprite->img).width;
    image_height = y_image + (terminix->zombies[i]->sprite->img).height;

    
    if((hero_x >= x_image && hero_x <= image_width) || ((hero_width >= x_image) && (hero_width <= image_width))){

      if((y_image <= hero_y && hero_y <= image_height) || ((y_image <= hero_height) && hero_height <= image_height)){
        return true;
      }
    }
  }

  return false;
}


void check_hero_direction(Hero * hero, Mouse * mouse){

  if((mouse->x - hero->x) == 0){
    if(mouse->y <= hero->y){
      hero->direction = up;
    }
    else{
      hero->direction = down;
    }

    return;
  }

  if(mouse->y == hero->y){
    if(mouse->x <= hero->x){
      hero->direction = left;
    }
    else{
      hero->direction = right;
    }

    return;   
  }
  
  float slope = -1.0 * (mouse->y - hero->y) / (mouse->x - hero->x);

  if(slope > -1 && slope < 1){

    if(mouse->x > hero->x){
      hero->direction = right;
    }
    else{
      hero->direction = left;
    }

  }
  else if(slope >-1 || slope > 1){

    if(mouse->y > hero->y){
      hero->direction = down;
    }
    else{
      hero->direction = up;
    }   
  }
}
