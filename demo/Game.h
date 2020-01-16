#include "Terminix.h"



int subscribe_periphericals(int *timer_irq, int *mouse_irq, int *keyboard_irq);
int unsubscribe_periphericals();
Terminix *get_current_terminix();
void update_positions();
void update_hero_position(Terminix * terminix);
void update_zombie_position(Zombie * zombie);
void update_mouse_img(Mouse * mouse);
void update_bullets_position(Bullet * bullet);
void update_zombies_position(Zombie * zombies[6], int n);
void render();
bool check_bullet_colisions(Bullet * bullet, Terminix * terminix);
bool check_heroe_collisions(Hero * hero, Terminix * terminix);
void check_hero_direction(Hero * hero, Mouse * mouse);
