#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
#include "VNH3SP30.h"
#include <Servo.h>
///////////
#include <string.h>
/////////////



#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

/*=========================================================================*/
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
    ////////////////////////////////////////////////
    #define BLE_READPACKET_TIMEOUT         500 
    /////////////////////////////////////////////////
/*=========================================================================*/

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

///////////////////////////////////////////////////////////////
// function prototypes over in packetparser.cpp
uint8_t readPacket(Adafruit_BLE *ble, uint16_t timeout);
float parsefloat(uint8_t *buffer);
void printHex(const uint8_t * data, const uint32_t numBytes);

// the packet buffer
extern uint8_t packetbuffer[];
//////////////////////////////////////////////////////////////////

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

VNH3SP30 Motor1 (PWMPin1, inAPin1, inBPin1);
VNH3SP30 Motor2 (PWMPin2, inAPin2, inBPin2);
VNH3SP30 Motor3 (PWMPin3, inAPin3, inBPin3);
VNH3SP30 Motor4 (PWMPin4, inAPin4, inBPin4);

Servo myservo;
///////////////////////////////////////SETUP///////////////////////////////////////////////////////
void setup() {
pinMode(52, OUTPUT);
digitalWrite(52, HIGH);
myservo.attach(13);
myservo.write(0);

 

/* Set up the Bluetooth */
while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Motor1.Stop();
  Motor2.Stop();
  Motor3.Stop();
  Motor4.Stop();
  Serial.println(F("RCP Robotic Base Control"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initializing the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////


int state = 0;
int i = 0;
int j = 0;
float errN = 300.0;
float errE = 300.0;
float rot_speed1 = 360/12.5; // deg/s (360 deg in 12.5 s)
float rot_speed2 = 360/7.5; // deg/s (360 deg in 7.5 s)
float rot_speed3 = 360/5.5; // deg/s (360 deg in 5.5 s)
float trans_speed1N = 10/19.3; // ft/s (10 feet in 19.3 s)
float trans_speed2N = 10/12.1; // ft/s (10 feet in 12.1 s)
float trans_speed3N = 10/9.0; // ft/s (10 feet in 9.0 s)
float trans_speed1E = 10/31.0; // ft/s (10 feet in 31.0 s)
float trans_speed2E = 10/15.0; // ft/s (10 feet in 15.0 s)
float trans_speed3E = 10/10.0; // ft/s (10 feet in 10.0 s)
float errN_last;
float errE_last;
float direct_last;
float time_trans;


  
//  int i;
  int x=150;//this variable is here used generally to dictate long the base will move in a given direction
//  int d=1;
  
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

/////////////////////////////////////////////////////////////LOOP////////////////////////////////////////////////////////////////
void loop() {

  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return;
  }
  // Some data was found, its in the buffer
  Serial.print(F("[Recv] "));
  Serial.println(ble.buffer);
  ble.waitForOK();

  if (errN == 300.0)
  {
    errN = atof(ble.buffer);
    Serial.println(fabs(errN));
  }
  else if (errE == 300.0)
  {
    errE = atof(ble.buffer);
    Serial.println(errE);
}
// if (i == 0)
// {
//    errN_last = errN;
//    errE_last = errE;
//    direct_last = 0;
//    i = 1;
// }

if (errN!=300.0 && errE!=300.0)
{
  if (fabs(errN) < .01)
  {
    state = 1;
    if (fabs(errE) < .01)
    {
        state = 2;
    }
  }
  else
  {
    state = 0;
  }
switch (state){
  case 0:
    if (fabs(errN)>1)
    {
      x = 255;    //FAST
      time_trans = fabs(errN)/trans_speed3N;
    }
    else if (fabs(errN >.5))
    {
      x = 125;    //MEDIUM
      time_trans = fabs(errN)/trans_speed2N;
    }
    else
    {
      x = 55;     //SLOW
      time_trans = fabs(errN)/trans_speed1N;
    }
    if (errN<0)
    {
      move_S(x);
    }
    else
    {
      move_N(x);
    }
    delay(time_trans*1000);
    stop_motors();
  break;
  case 1:
    if (fabs(errE)>1)
    {
      x = 255;    //FAST
      time_trans = fabs(errE)/trans_speed3E;
    }
    else if (fabs(errE >.5))
    {
      x = 125;    //MEDIUM
      time_trans = fabs(errE)/trans_speed2E;
    }
    else
    {
      x = 55;     //SLOW
      time_trans = fabs(errE)/trans_speed1E;
    }
    if (errE<0)
    {
      move_W(x);
    }
    else
    {
      move_E(x);
    }
    delay(time_trans*1000);
    stop_motors();
  break;
  case 3:
    stop_motors();
    delay(250);
    myservo.write(155);
    delay(500);
    myservo.write(0);
    delay(500);
  break;
  default:
  break;
}

errN = 300.0;
errE = 300.0;


}


 uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);
  if (len == 0) return;


  if (packetbuffer[1] == 'B') {
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    
    if (pressed) {
      if(buttnum==5 ||buttnum=='5'){
        move_N(200);
      }
      else if(buttnum==6||buttnum=='6'){
        move_S(200);
      }
      else if(buttnum==7||buttnum=='7'){
        move_W(200);
      }
      else if(buttnum==8||buttnum=='8'){
        move_E(200);
      }
      else{
        stop_motors();
      }
           
    }  
    else {
      stop_motors();
    }
  }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////


bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds

  
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (!Serial.available()) && !timeout.expired() ) { delay(1); }

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && (Serial.available()) );

  return true;
}

