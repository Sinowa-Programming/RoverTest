// RoverTest.ino
// FGCU Computing & Software Engineering
// Prof. Allen
// Test the basic Rover robot interfaces. Uses a set of states
// to run the wheels through each of the basic movements and
// adjusts the head to point in the direction of movement. The
// Serial inteface is used to output direction of movement, along
// with direction of the head and distance measured.
// ------------------------------------------------------------

// #include <LiquidCrystal.h>

#include "RoverHead.h"
#include "RoverWheels.h"
#include "StopWatch.h"

// LCD
// const int rs = 12, en = 11, d4 = 3, d5 = 2, d6 = 1, d7 = 0;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


fgcu::FourPin motorPinsLeft { A0, A1, A2, A3};
fgcu::FourPin motorPinsRight { 4, 5, 6, 7};
float speed = 600.f;
fgcu::RoverWheels wheels{motorPinsLeft, motorPinsRight, speed};

const byte ServoPin = 9;
const byte EchoPin = A5;
const byte TriggerPin = A4;

fgcu::RoverHead head{EchoPin, TriggerPin, ServoPin};

const float TURN_DEGREE = 1;  // Turn complete 90 degrees

enum STATE {
  NORMAL = 0,
  FORCE_FORWARD = 1
};

byte curState = NORMAL;

enum ANGLE {
  LEFT = 8,
  STRAIGHT = 98,
  RIGHT = 180
};

// Specific code
// All in inches
const float moveDist = 4;
const float minDist = 10;

void setup() {
  // lcd.begin(16,2);
  // lcd.write("Hello world!");

  delay(2000);
  head.turnHead(STRAIGHT);
  while(!head.run());
  // lcd.clear();
}


word straightDist{0};
void loop() {
  wheels.run();
  if(!wheels.isMoving()) {
    switch(curState) {
      case NORMAL: {
        head.turnHead((byte)STRAIGHT);
        while(head.isMeasuring()) {
          head.run();
        }
        straightDist = head.getDistance();

        if(straightDist < minDist) {
          head.turnHead((byte)RIGHT);
          while(head.isMeasuring()) {
            head.run();
          }
          word rDist = head.getDistance();
        
          head.turnHead((byte)LEFT);
          while(head.isMeasuring()) {
            head.run();
          }
          word lDist = head.getDistance();

          if(rDist > lDist) { // left dist > right dist
            wheels.turnLeft(TURN_DEGREE);
            curState = FORCE_FORWARD;
            straightDist = rDist;
          } else {
            wheels.turnRight(TURN_DEGREE);
            curState = FORCE_FORWARD;
            straightDist = lDist;
          }
        } else {
          curState = FORCE_FORWARD; // Each full rotation is 8 inches
        }
        break;
      }
      case FORCE_FORWARD: {
        wheels.moveForward((straightDist*.60)/8.f); // Each full rotation is 8 inches
        curState = NORMAL;
      }
    }
  }
};
