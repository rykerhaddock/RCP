#include <Arduino.h>
#include "motor_commands.h"

const int inAPin1 = 22;
const int inBPin1 = 23;
const int PWMPin1 = 4;

const int inAPin2 = 34;
const int inBPin2 = 35;
const int PWMPin2 = 2;

const int inAPin3 = 24;
const int inBPin3 = 25;
const int PWMPin3 = 3;

const int inAPin4 = 8;
const int inBPin4 = 9;
const int PWMPin4 = 5;

VNH3SP30 Motor1 (PWMPin1, inAPin1, inBPin1);
VNH3SP30 Motor2 (PWMPin2, inAPin2, inBPin2);
VNH3SP30 Motor3 (PWMPin3, inAPin3, inBPin3);
VNH3SP30 Motor4 (PWMPin4, inAPin4, inBPin4);

void stop_motors(){
  Motor1.Stop();
  Motor2.Stop();
  Motor3.Stop();
  Motor4.Stop();
  }

  void rotate_clockwise(int k){
  Motor1.Move(k,HIGH);
  Motor2.Move(k,HIGH);
  Motor3.Move(k,HIGH);
  Motor4.Move(k,HIGH);
  }
  
  void rotate_counterclockwise(int k){
  Motor1.Move(k,LOW);
  Motor2.Move(k,LOW);
  Motor3.Move(k,LOW);
  Motor4.Move(k,LOW);
  } 

void move_N(int k){
  Motor1.Move(k,HIGH);
  Motor2.Move(k,HIGH);
  Motor3.Move(k,LOW);
  Motor4.Move(k,LOW);
}  

  void move_S(int k){
  Motor1.Move(k,LOW);
  Motor2.Move(k,LOW);
  Motor3.Move(k,HIGH);
  Motor4.Move(k,HIGH);
  } 
  void move_E(int k){ 
  Motor1.Move(k,HIGH);
  Motor2.Move(k,LOW);
  Motor3.Move(k,LOW);
  Motor4.Move(k,HIGH);
  } 
void move_W(int k){
  Motor1.Move(k,LOW);
  Motor2.Move(k,HIGH);
  Motor3.Move(k,HIGH);
  Motor4.Move(k,LOW);
}

void move_NW(int k){
  Motor1.Stop();
  Motor2.Move(k,HIGH);
  Motor3.Stop();
  Motor4.Move(k,LOW);
  }
   
void move_SE(int k){
  Motor1.Stop();
  Motor2.Move(k,LOW);
  Motor3.Stop();
  Motor4.Move(k,HIGH);
  }
   
void move_NE(int k){
  Motor1.Move(k,HIGH);
  Motor2.Stop();
  Motor3.Move(k,LOW);  
  Motor4.Stop();
  }

void move_SW(int k){
  Motor1.Move(k,LOW);
  Motor2.Stop();
  Motor3.Move(k,HIGH);  
  Motor4.Stop();
  }
  
void demo1(){
  int y = 2000;
  int z = 200;
  while(!Serial1.available())
  {
    move_N(z);
    delay(y);
    move_E(z);
    delay(y);
    move_S(z);
    delay(y);
    move_W(z);
    delay(y);
    stop_motors();
    delay(y);
  }
}

void demo2(){
  int y = 2000;
  int z = 200;
  while(!Serial1.available())
  {
    move_NE(z);
    delay(y);
    move_SW(z);
    delay(y);
    stop_motors();
    delay(y);
  }
}
