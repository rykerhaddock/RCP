#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"
#include "VNH3SP30.h"


#if SOFTWARE_SERIAL_AVAILABLE
  #include <SoftwareSerial.h>
#endif

/*=========================================================================*/
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);

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

VNH3SP30 Motor1 (PWMPin1, inAPin1, inBPin1);
VNH3SP30 Motor2 (PWMPin2, inAPin2, inBPin2);
VNH3SP30 Motor3 (PWMPin3, inAPin3, inBPin3);
VNH3SP30 Motor4 (PWMPin4, inAPin4, inBPin4);

void setup() {

/* Set up the Motor */
//  pinMode(inAPin1, OUTPUT);
//  pinMode(inBPin1, OUTPUT);
//  pinMode(PWMPin1, OUTPUT);
//  pinMode(inAPin2, OUTPUT);
//  pinMode(inBPin2, OUTPUT);
//  pinMode(PWMPin2, OUTPUT);
//  pinMode(inAPin3, OUTPUT);
//  pinMode(inBPin3, OUTPUT);
//  pinMode(PWMPin3, OUTPUT);
//  pinMode(inAPin4, OUTPUT);
//  pinMode(inBPin4, OUTPUT);
//  pinMode(PWMPin4, OUTPUT);

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

// Check for user input
  char inputs[BUFSIZE+1];

  if ( getUserInput(inputs, BUFSIZE) )
  {
    // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);
  
    // check response status
    if (! ble.waitForOK() ) {
      Serial.println(F("Failed to send?"));
    }
  }

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

    if (ble.buffer[0] == '1')
    {
      x=50;
    }
    else if (ble.buffer[0] == '2')
    {
      x = 125;
    }
    else if (ble.buffer[0] == '3')
    {
      x = 255;
    }
    else
    {
      x = 0;
    }

    if (ble.buffer[1] == 'n')
    {
      if (ble.buffer[2] == 'w')
      {
        move_NW(x);
      }
      else if (ble.buffer[2]=='e')
      {
        move_NE(x);
      }
      else
      {
        move_forward(x);
      }
    }
    else if (ble.buffer[1] == 'e')
    {
      move_right(x);
    }
    else if (ble.buffer[1] == 's')
    {
      if (ble.buffer[2] == 'w')
      {
        move_SW(x);
      }
      else if (ble.buffer[2]=='e')
      {
        move_SE(x);
      }
      else
      {
        move_backward(x);
      }    
    }
    
    else if (ble.buffer[1] == 'w')
    {
      move_left(x);
    }

    else if (ble.buffer[1] == 'c'){
      if (ble.buffer[2] == 'c'){
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



  ble.waitForOK();
}

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
