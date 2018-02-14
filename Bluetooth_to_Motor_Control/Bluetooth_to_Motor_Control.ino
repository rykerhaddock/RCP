#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"
#include "BluefruitConfig.h"

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

const int inAPin1 = 4;
const int inBPin1 = 2;
const int PWMPin1 = 3;

const int inAPin2 = 8;
const int inBPin2 = 10;
const int PWMPin2 = 9;

const int inAPin3 = 7;
const int inBPin3 = 5;
const int PWMPin3 = 6;

const int inAPin4 = 12;
const int inBPin4 = 13;
const int PWMPin4 = 11;


void setup() {

/* Set up the Motor */
  pinMode(inAPin1, OUTPUT);
  pinMode(inBPin1, OUTPUT);
  pinMode(PWMPin1, OUTPUT);
  pinMode(inAPin2, OUTPUT);
  pinMode(inBPin2, OUTPUT);
  pinMode(PWMPin2, OUTPUT);
  pinMode(inAPin3, OUTPUT);
  pinMode(inBPin3, OUTPUT);
  pinMode(PWMPin3, OUTPUT);
  pinMode(inAPin4, OUTPUT);
  pinMode(inBPin4, OUTPUT);
  pinMode(PWMPin4, OUTPUT);

/* Set up the Bluetooth */
while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
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
  digitalWrite(inAPin1, LOW); //stop motors can be both low or high
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, LOW);
  digitalWrite(inBPin4, LOW);
  }

  void rotate_clockwise(int k){
  digitalWrite(inAPin1, HIGH); // move one direction, ROTATES CLOCKWISE
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, HIGH);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, HIGH);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, HIGH);
  digitalWrite(inBPin4, LOW);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  }
  
  void rotate_counterclockwise(int k){
  digitalWrite(inBPin1, HIGH); // move other direction, ROTATES COUNTERCLOCKWISE
  digitalWrite(inAPin1, LOW);
  digitalWrite(inBPin2, HIGH);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin3, HIGH);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin4, HIGH);
  digitalWrite(inAPin4, LOW);
  
  for (i = 0; i < k; i++){
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  } 


void move_forward(int k){
  digitalWrite(inAPin1, LOW); // move one direction FORWARD
  digitalWrite(inBPin1, HIGH);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin2, HIGH);
  digitalWrite(inAPin3, HIGH);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, HIGH);
  digitalWrite(inBPin4, LOW);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
  }
}  

  void move_backward(int k){
  digitalWrite(inAPin1, HIGH); // move one direction BACKWARDS
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, HIGH);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin3, HIGH);
  digitalWrite(inAPin4, LOW);
  digitalWrite(inBPin4, HIGH);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  } 
  void move_right(int k){ 
  digitalWrite(inAPin1, HIGH); // move one direction RIGHT
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin2, HIGH);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin3, HIGH);
  digitalWrite(inAPin4, HIGH);
  digitalWrite(inBPin4, LOW);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  } 
void move_left(int k){
  digitalWrite(inAPin1, LOW); // move one direction LEFT
  digitalWrite(inBPin1, HIGH);
  digitalWrite(inAPin2, HIGH);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, HIGH);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, LOW);
  digitalWrite(inBPin4, HIGH);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
  }
}


void move_NE(int k){
  digitalWrite(inAPin1, LOW); // move one direction RIGHT FORWARD DIAGONAL
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin2, HIGH);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, HIGH);
  digitalWrite(inBPin4, LOW);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  }
   
void move_SW(int k){
  digitalWrite(inAPin1, LOW); // move one direction LEFT BACKWARD DIAGONAL
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, HIGH);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, LOW);
  digitalWrite(inBPin4, HIGH);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  }
   

void move_NW(int k){
  digitalWrite(inAPin1, LOW); // move one direction LEFT FORWARD DIAGONAL
  digitalWrite(inBPin1, HIGH);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, HIGH);
  digitalWrite(inBPin3, LOW);
  digitalWrite(inAPin4, LOW);
  digitalWrite(inBPin4, LOW);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
  }

void move_SE(int k){
  digitalWrite(inAPin1, HIGH); // move one direction RIGHT BACKWARD DIAGONAL
  digitalWrite(inBPin1, LOW);
  digitalWrite(inAPin2, LOW);
  digitalWrite(inBPin2, LOW);
  digitalWrite(inAPin3, LOW);
  digitalWrite(inBPin3, HIGH);
  digitalWrite(inAPin4, LOW);
  digitalWrite(inBPin4, LOW);
  
  for (i = 0; i < k; i++){ 
     analogWrite(PWMPin1, i);
     analogWrite(PWMPin2, i);
     analogWrite(PWMPin3, i);
     analogWrite(PWMPin4, i);
     delay(d);
    }
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

// Enables Control From the Laptop
//    if (*inputs == 'n')
//    {
//      move_forward(150);
//    }
//    else if (*inputs == 'e')
//    {
//      move_right(150);
//    }
//    else if (*inputs == 's')
//    {
//      move_backward(150);
//    }
//    else if (*inputs == 'w')
//    {
//      move_left(150);
//    }
//    else
//    {
//      stop_motors();
//    }
  
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

    if (ble.buffer[0] == 'n')
    {
      if (ble.buffer[1] == 'w')
      {
        move_NW(x);
      }
      else if (ble.buffer[1]=='e')
      {
        move_NE(x);
      }
      else
      {
        move_forward(x);
      }
    }
    else if (ble.buffer[0] == 'e')
    {
      move_right(x);
    }
    else if (ble.buffer[0] == 's')
    {
      if (ble.buffer[1] == 'w')
      {
        move_SW(x);
      }
      else if (ble.buffer[1]=='e')
      {
        move_SE(x);
      }
      else
      {
        move_backward(x);
      }    
    }
    
    else if (ble.buffer[0] == 'w')
    {
      move_left(x);
    }

    else if (ble.buffer[0] == 'c'){
      if (ble.buffer[1] == 'c'){
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
