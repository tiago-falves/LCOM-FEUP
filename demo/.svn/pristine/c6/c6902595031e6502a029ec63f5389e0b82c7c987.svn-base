#include "Sprite.h"


 #define HERO_START_POSITION_X 400
 #define HERO_START_POSITION_Y 400

typedef struct {

  Sprite * sprite;
  uint16_t x_direction;
  uint16_t y_direction;
  xpm_image_t bullet_img;

} Bullet;

enum direction {up, left, right, down};

typedef struct {

  uint16_t x;
  uint16_t y;
  uint16_t new_x;
  uint16_t new_y;
  uint16_t x_direction;
  uint16_t y_direction;
  xpm_image_t hero_img;
  xpm_image_t  sprites[4];
  enum direction direction;
  Bullet * bullet;

} Hero;


Hero * create_hero();
void get_bullet_coords(Hero* hero, uint16_t * x, uint16_t * y);


void hero_move_up(Hero * hero_ptr);
void hero_move_down(Hero * hero_ptr);
void hero_move_left(Hero * hero_ptr);
void hero_move_right(Hero * hero_ptr);
Bullet * shoot(Hero * hero_ptr);

void get_bullet_direction(Hero * hero, uint16_t * x, uint16_t * y);

void destroy_bullet(Bullet ** bullet);

void update_xpm(Hero * hero);



