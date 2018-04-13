
#ifndef motor_commands_H
#define motor_commands_H

#include <Arduino.h>
#include "VNH3SP30.h"

void stop_motors();

void rotate_clockwise(int k);
  
void rotate_counterclockwise(int k);

void move_N(int k);

void move_S(int k);

void move_E(int k);

void move_W(int k);

void move_NW(int k);
   
void move_SE(int k);
   
void move_NE(int k);

void move_SW(int k);
  
void demo1();

void demo2();


#endif

