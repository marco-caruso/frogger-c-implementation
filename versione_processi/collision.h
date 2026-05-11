//collision.h
#ifndef COLLISION_H
#define COLLISION_H

#include "definizioni.h"

bool frog_is_drowning();
bool croc_on_both();
bool croc_on_right();
bool croc_on_left();
bool check_croc_for_frog(int, int);//non c'è
bool frog_on_water();
bool frog_is_out_of_bounds();
bool croc_is_out_of_bounds(struct msg);
//bool check_frog_win();
bool is_frog_on_croc();
bool frog_collision(int, int);
bool bullet_is_out_of_bounds(int);

#endif //COLLISION_H
