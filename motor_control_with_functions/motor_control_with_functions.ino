/*********************************************************
This code uses an Arduino uno to command one SMD PWM Motor COntroller (TE-294-001).
***********************************************************/
///constants correlating to the pins on the arduino that are connected to each motor
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

/////direction comments are assuming motors 1 & 4 are the "front" of the vehicle
////and the base is "upside down", meaning the motors and controllers are on the top

void setup() {//initializes the pins on the arduino to be outputs

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
}


  int i;
  int x=150;//this variable is here used generally to dictate long the base will move in a given direction
  
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
     delay(10);
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
     delay(10);
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
     delay(10);
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
     delay(10);
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
     delay(10);
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
     delay(10);
  }
}


void move_diagonal_RF(int k){
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
     delay(10);
  }
  }
   
void move_diagonal_LB(int k){
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
     delay(10);
  }
  }
   

void move_diagonal_LF(int k){
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
     delay(10);
  }
  }

void move_diagonal_RB(int k){
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
     delay(10);
  }
  }




void loop(){
  stop_motors();
  delay(2000);
  move_forward(x);
  delay(2000);
  stop_motors();
  delay(2000);
  move_backward(x);
  delay(2000);
  stop_motors();
  delay(2000);
  move_left(x);
  delay(2000);
  stop_motors();
  delay(2000);
  move_right(x);
  delay(2000);
  stop_motors();
  delay(2000);

  }
