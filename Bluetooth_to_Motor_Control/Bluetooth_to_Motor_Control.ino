#include <Arduino.h>
#include "VNH3SP30.h"
#include <Servo.h>
#include <string.h>
#include <SoftwareSerial.h>
#include "motor_commands.h"

/*=========================================================================*/
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
const int Rx_b = 19;
const int Tx_b = 18;

//for servo control
Servo base;
int analogPin = 3;
float volt = 0.0;
float resist = 33;
float cur = 0;

String inputString = "";
boolean inputStringReceived = false;

////////////////////////////////////////////////////////////////
void setup() {
pinMode(Rx_b, INPUT);  
digitalWrite(Rx_b, HIGH);
pinMode(52, OUTPUT);
digitalWrite(52, HIGH);
base.attach(6);
base.write(0);

/* Set up the Bluetooth */
while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(9600);
  Serial1.begin(9600);
  stop_motors();
  Serial.println(F("RCP Robotic Base Control"));
  Serial.println(F("---------------------------------------"));
  // reserve 200 bytes for the inputString
  inputString.reserve(200);
}
////////////////////////////////////////////////////////////////
  
  int i;
  int x=150;//this variable is here used generally to dictate long the base will move in a given direction
  int d=1;
  int n = 0;
  int k = 0;
  
void loop() {
  if (inputStringReceived){
    command_motors(inputString);
    inputString="";
    inputStringReceived = false;    
  }

  if (n>10000){
    if (k<50){
        k = k+1;
        n = 0;
      }
      else{
        stop_motors();
      }
  }
    n = n+1;
}

void command_motors(String inputString1){
  // Some data was found, its in the Serial.buffer
  Serial.println(inputString1);
  n = 0;
  k = 0;
    if (inputString1[0] == 'x')
    {
     motor();
     delay(3000);
    }  
    else if (inputString1[0]=='d')
    {
      if (inputString1[1] == '1')
        {
          demo1();
        }
        else if (inputString1[1] == '2')
        {
          demo2();
        }
    }

    else if (inputString1[0] == '1')
    {
      x=75;
    }
    else if (inputString1[0] == '2')
    {
      x = 75;
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
        move_N(x);
      }
    }
    else if (inputString1[1] == 'e')
    {
      move_E(x);
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
        move_S(x);
      }    
    }
    
    else if (inputString1[1] == 'w')
    {
      move_W(x);
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
    if (inputString1[0] == '1')
    {
      delay(75);
      stop_motors();
      delay(1000);
    }
}

void motor()
{
  /* Servo Motor commanded to fully extend
   * While extending the deriviative of the voltage through the motor is measured
   * The voltage measuremtns are averaged to make a smoother curve
   * The voltage spikes when an ostacle is hit
   * When the voltage spikes the motor stops moving
   */
  base.write(180); //motor commanded to fully extend 
  float average = 0;//initialize current average voltage value
  float aver_prev = 350; //initialize previous value used to find dervitive
  float derive_av = 0; //initialize derivitive value
  delay(100);//timing to allign servo motor angle with code predicted value 
  for (int cur=1; cur<180; cur = cur +2) //cur is predicted servo angle
  {
    average = 0;//reinitalize current average
    //this for statement reads the voltage several times and averages it
    for (int i=1;i<100;i++)
    {
        volt = analogRead(analogPin);
        average = average+volt;
    }
    delay(2); // this alligns the loop time with the servo motion
    average = average/9.0;//technically should divide by 99, but 9 makes a times ten gain for aproximately a ten times gain
    derive_av = average-aver_prev;
    aver_prev = average;
    //Serial.println(derive_av);
    if (derive_av >70 &&cur>20)
    {
      base.write(0);//stop at current predicted value
      cur = 180; // exit loop because condition is met
    }
  }
  base.write(0);
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



