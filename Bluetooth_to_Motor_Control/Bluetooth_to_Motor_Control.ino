#include <Arduino.h>
#include "VNH3SP30.h"
#include <Servo.h>
#include <string.h>
#include <SoftwareSerial.h>

/*=========================================================================*/
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

const int inAPin1 = 22;
const int inBPin1 = 23;
const int PWMPin1 = 4;

const int inAPin2 = 26;
const int inBPin2 = 27;
const int PWMPin2 = 2;

const int inAPin3 = 24;
const int inBPin3 = 25;
const int PWMPin3 = 3;

const int inAPin4 = 8;
const int inBPin4 = 9;
const int PWMPin4 = 5;

const int Rx_b = 19;
const int Tx_b = 18;

VNH3SP30 Motor1 (PWMPin1, inAPin1, inBPin1);
VNH3SP30 Motor2 (PWMPin2, inAPin2, inBPin2);
VNH3SP30 Motor3 (PWMPin3, inAPin3, inBPin3);
VNH3SP30 Motor4 (PWMPin4, inAPin4, inBPin4);

Servo myservo;
String inputString = "";
boolean inputStringReceived = false;

////////////////////////////////////////////////////////////////
void setup() {
pinMode(Rx_b, INPUT);  
digitalWrite(Rx_b, HIGH);
pinMode(52, OUTPUT);
digitalWrite(52, HIGH);
myservo.attach(13);
myservo.write(0);

/* Set up the Bluetooth */
while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(9600);
  Serial1.begin(9600);
  Motor1.Stop();
  Motor2.Stop();
  Motor3.Stop();
  Motor4.Stop();
  Serial.println(F("RCP Robotic Base Control"));
  Serial.println(F("---------------------------------------"));
  // reserve 200 bytes for the inputString
  inputString.reserve(200);
}
////////////////////////////////////////////////////////////////
  
  int i;
  int x=150;//this variable is here used generally to dictate long the base will move in a given direction
  int d=1;
  
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

void move_forward(int k){
  Motor1.Move(k,HIGH);
  Motor2.Move(k,HIGH);
  Motor3.Move(k,LOW);
  Motor4.Move(k,LOW);
}  

  void move_backward(int k){
  Motor1.Move(k,LOW);
  Motor2.Move(k,LOW);
  Motor3.Move(k,HIGH);
  Motor4.Move(k,HIGH);
  } 
  void move_right(int k){ 
  Motor1.Move(k,HIGH);
  Motor2.Move(k,LOW);
  Motor3.Move(k,LOW);
  Motor4.Move(k,HIGH);
  } 
void move_left(int k){
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

void loop() {
  if (inputStringReceived){
    Serial.println(inputString);
    command_motors(inputString);
    inputString="";
    inputStringReceived = false;    
  }
}

void command_motors(String inputString1){
  // Some data was found, its in the Serial.buffer
  Serial.println(inputString1);

    if (inputString1[0] == 'x')
    {
      stop_motors();
      delay(250);
      myservo.write(155);
      delay(500);
      myservo.write(0);
      delay(500); 
    }  

    else if (inputString1[0] == '1')
    {
      x=75;
    }
    else if (inputString1[0] == '2')
    {
      x = 125;
    }
    else if (inputString1[0] == '3')
    {
      x = 255;
    }
    else
    {
      x = 0;
    }

    if (inputString1[1] == 'n')
    {
      if (inputString1[2] == 'w')
      {
        move_NW(x);
      }
      else if (inputString1[2]=='e')
      {
        move_NE(x);
      }
      else
      {
        move_forward(x);
      }
    }
    else if (inputString1[1] == 'e')
    {
      move_right(x);
    }
    else if (inputString1[1] == 's')
    {
      if (inputString1[2] == 'w')
      {
        move_SW(x);
      }
      else if (inputString1[2]=='e')
      {
        move_SE(x);
      }
      else
      {
        move_backward(x);
      }    
    }
    
    else if (inputString1[1] == 'w')
    {
      move_left(x);
    }

    else if (inputString1[1] == 'c'){
      if (inputString1[2] == 'c'){
        rotate_counterclockwise(x);
      }
      else{
        rotate_clockwise(x);
      }     
    }
    else
    {
      stop_motors();
    }
}

void serialEvent1() {
  static boolean recvInProgress = false;
    char startMarker = '<';
    char endMarker = '>';
 
  while (Serial1.available() > 0) {
    char inChar = Serial1.read();

    if (recvInProgress == true) {       
      if (inChar == startMarker) {
        inputString = "";
      }     
      else if (inChar != endMarker) {
          inputString += inChar;
      }
      else {
          recvInProgress = false;
          inputStringReceived = true;
      }     
    }
    else if (inChar == startMarker) {
        recvInProgress = true;
    }
  }
}
