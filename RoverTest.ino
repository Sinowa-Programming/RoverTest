// RoverTest.ino
// FGCU Computing & Software Engineering
// Siang Chin & Sahbah Gonzalez
// Navigate the rover though a simple course using an ultrasonic sensor
// to detect walls and 2 steppers motors to move in a controlled fashion.
// ------------------------------------------------------------

#include "RoverHead.h"
#include "RoverWheels.h"
#include "StopWatch.h"


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
  delay(2000);
  head.turnHead(STRAIGHT);
  while(!head.run());
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
            curState = FORCE_FORWARD;  // Make sure the rover moves forward after turning
            straightDist = rDist;
          } else {
            wheels.turnRight(TURN_DEGREE);
            curState = FORCE_FORWARD;   // Make sure the rover moves forward after turning
            straightDist = lDist;
          }
        } else {
          curState = FORCE_FORWARD; // Each full rotation is 8 inches
        }
        break;
      }
      case FORCE_FORWARD: {
        wheels.moveForward((straightDist*.60)/8.f); // Each full rotation is 8 inches. Move forward for 60% of the detected distance.
        curState = NORMAL;
      }
    }
  }
};
