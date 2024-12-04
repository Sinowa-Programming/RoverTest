// RoverTest.ino
// FGCU Computing & Software Engineering
// Prof. Allen
// Test the basic Rover robot interfaces. Uses a set of states
// to run the wheels through each of the basic movements and
// adjusts the head to point in the direction of movement. The
// Serial inteface is used to output direction of movement, along
// with direction of the head and distance measured.
// ------------------------------------------------------------

#include <LiquidCrystal.h>

#include "RoverHead.h"
#include "RoverWheels.h"
#include "StopWatch.h"

// LCD
const int rs = 12, en = 11, d4 = 3, d5 = 2, d6 = 1, d7 = 0;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


fgcu::FourPin motorPinsLeft { A0, A1, A2, A3};
fgcu::FourPin motorPinsRight { 4, 5, 6, 7};
float speed = 600.f;
fgcu::RoverWheels wheels{motorPinsLeft, motorPinsRight, speed};

const byte ServoPin = 9;
const byte EchoPin = A5;
const byte TriggerPin = A4;

fgcu::RoverHead head{EchoPin, TriggerPin, ServoPin};

enum ANGLE {
  LEFT = 8,
  STRAIGHT = 98,
  RIGHT = 188
};

// Specific code
// All in inches
const float moveDist = 4;
const float minDist = 10;

void setup() {
  lcd.begin(16,2);
  lcd.write("Hello world!");

  delay(2000);

  lcd.clear();

  head.turnHead(STRAIGHT);
}


word straightDist{0};
void loop() {
  wheels.run();
  if(!wheels.isMoving()) {
    straightDist = getDistanceAtAngle(STRAIGHT);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.setCursor(10, 0);
    lcd.print(straightDist);

    if(straightDist < minDist) {
      if(getDistanceAtAngle(RIGHT) > getDistanceAtAngle(LEFT)) { // left dist > right dist
        wheels.turnLeft(1.2);
      } else {
        wheels.turnRight(1.2);
      }
    } else {
      wheels.moveForward(moveDist/8.f); // Each full rotation is 8 inches
    }
  }

};

word getDistanceAtAngle(byte angle) {
    // Right check
  head.turnHead(angle);
  while(head.isMeasuring()) {
    head.run();
  }
  return head.getDistance();
}