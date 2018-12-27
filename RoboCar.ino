/******************************************************************************
TestRun.ino
TB6612FNG H-Bridge Motor Driver Example code
Michelle @ SparkFun Electronics
8/20/16
https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library

Uses 2 motors to show examples of the functions in the library.  This causes
a robot to do a little 'jig'.  Each movement has an equal and opposite movement
so assuming your motors are balanced the bot should end up at the same place it
started.

Resources:
TB6612 SparkFun Library

Development environment specifics:
Developed on Arduino 1.6.4
Developed with ROB-9457
******************************************************************************/

// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define PWMA 3
#define AIN2 4
#define AIN1 5
#define STBY 6
#define BIN1 7
#define BIN2 8
#define PWMB 9

#define SENSOR_FORWARD 16
#define SENSOR_LEFT 17
#define SENSOR_RIGHT 15

#define DIR_STOP 0
#define DIR_FORWARD 1
#define DIR_BACKWARD 2
#define DIR_LEFT 3
#define DIR_RIGHT 4

#define SPEED_0 0
#define SPEED_1 1
#define SPEED_2 2
#define SPEED_3 3
#define SPEED_4 4
#define SPEED_5 5


int movementDirection = DIR_FORWARD;
int nextDirection = DIR_FORWARD;
int motorSpeed = SPEED_2;
int nextSpeed = SPEED_2;
unsigned long millisTillEndOfRoutine = 0;
unsigned long nextMillisTillEndOfRoutine = 0;

// the below arrays is to implement a low pass filter (debounce? the sensors)
bool ba_right[5] = {false, false, false, false, false};
bool ba_left[5] = {false, false, false, false, false};
bool ba_forward[5] = {false, false, false, false, false};

// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 49;
const int offsetB = 50; //the speed will change between 0-10 ~0-250

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

byte i=0;

void setup()
{
  pinMode(SENSOR_FORWARD, INPUT_PULLUP);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);
  pinMode(SENSOR_RIGHT, INPUT_PULLUP);

  delay(3000);

//  Serial.begin(115200);   // Debugging only
}


void loop()
{
//  Serial.print(F("millisTillEndOfRoutine = "));
//  Serial.print(millisTillEndOfRoutine);
//  Serial.print(F(" , Speed = "));
//  Serial.print(motorSpeed);
//  Serial.print(F(" , Direction = "));
//  Serial.println(movementDirection);

  //read the sensors
  ba_right[i] = digitalRead(SENSOR_RIGHT) == LOW;
  ba_left[i] = digitalRead(SENSOR_LEFT) == LOW;
  ba_forward[i] = digitalRead(SENSOR_FORWARD) == LOW;
  i++;
  if (i>4) i=0;
  
  int millisNow = millis();
  bool b_right = ba_right[0] && ba_right[2] && ba_right[2] && ba_right[3] && ba_right[4];
  bool b_left =  ba_left[0] && ba_left[2] && ba_left[2] && ba_left[3] && ba_left[4];
  bool b_forward =  ba_forward[0] && ba_forward[2] && ba_forward[2] && ba_forward[3] && ba_forward[4];

  if (b_right || b_left || b_forward){
    
    if (movementDirection == DIR_FORWARD){
//      back(motor1, motor2, SPEED_1);
//      delay(750);
      // figure out where to turn to
      if(b_right){
        //obstacle on the right
        movementDirection = DIR_LEFT;    
        motorSpeed = SPEED_2;
      }else{
        //obstacle on the left
        movementDirection = DIR_RIGHT;    
        motorSpeed = SPEED_2;
      }
    }
  }else{
    if (movementDirection != DIR_FORWARD){
      delay(750);
    }
    //no obstacle -- full speed ahead
    movementDirection = DIR_FORWARD;
    motorSpeed = SPEED_3;
  }
  
//  Serial.print(F("Speed = "));
//  Serial.print(motorSpeed);
//  Serial.print(F(" , Direction = "));
//  Serial.println(movementDirection);

  if(movementDirection == DIR_FORWARD){
//    Serial.println(F(" DIR_FORWARD"));
    forward(motor1, motor2, motorSpeed);
  }else if(movementDirection == DIR_BACKWARD){
//    Serial.println(F(" DIR_BACKWARD"));
    back(motor1, motor2, motorSpeed);
  }else if(movementDirection == DIR_LEFT){
//    Serial.println(F(" DIR_LEFT"));
    left(motor1, motor2, motorSpeed);
  }else if(movementDirection == DIR_RIGHT){
//    Serial.println(F(" DIR_RIGHT"));
    right(motor1, motor2, motorSpeed);
  }else {
//    Serial.println(F(" BREAK"));
    brake(motor1, motor2);
  }

  delay(50);
}
