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


int direction = 1;
int speed = 5;
bool brakeOn = false;

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

void setup()
{
  pinMode(SENSOR_FORWARD, INPUT_PULLUP);
  pinMode(SENSOR_LEFT, INPUT_PULLUP);
  pinMode(SENSOR_RIGHT, INPUT_PULLUP);

  delay(3000);
}


void loop()
{
  bool b_right = digitalRead(SENSOR_RIGHT) ==LOW;
  bool b_left = digitalRead(SENSOR_LEFT) ==LOW;
  bool b_forward = digitalRead(SENSOR_FORWARD) ==LOW;
  if( b_right && b_left || b_forward){
    //the way is blocked, time to skid and stop
    if (!brakeOn){
      left(motor1, motor2, SPEED_2);
      delay(500);
      brake(motor1, motor2);
      delay(1000);
      brakeOn = true;
    }
    speed = SPEED_3;
    direction = DIR_LEFT;    
  }else if(b_right && b_forward){
    //obstacle on the right
    left(motor1, motor2, SPEED_2);
    delay(200);
    speed = SPEED_2;
    direction = DIR_LEFT;    
  }else if(b_left && b_forward){
    //obstacle on the left
    right(motor1, motor2, SPEED_2);
    delay(200);
    speed = SPEED_2;
    direction = DIR_RIGHT;    
  }else if(b_right){
    //obstacle on the right
    direction = DIR_LEFT;    
    speed = SPEED_3;
  }else if(b_left){
    //obstacle on the right
    direction = DIR_RIGHT;    
    speed = SPEED_3;
  } else {
    //no obstacle -- full speed ahead
    direction = DIR_FORWARD;
    speed = SPEED_4;
  }
  
  if(direction == DIR_FORWARD){
    forward(motor1, motor2, speed);
  }else if(direction == DIR_BACKWARD){
    back(motor1, motor2, speed);
  }else if(direction == DIR_LEFT){
    left(motor1, motor2, speed);
  }else if(direction == DIR_RIGHT){
    right(motor1, motor2, speed);
  }else {
    brake(motor1, motor2);
  }
    
  delay(50);
}
