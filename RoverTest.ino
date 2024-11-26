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

enum RoverWheelState {
  Stop,
  Straight,
  ShortStraight,
  CorrectLeft,
  TurnLeft,
  CorrectRight,
  TurnRight,
  Backup
};

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



void setup() {
  lcd.begin(16,2);
  lcd.write("Hello world!");

  delay(2000);

  head.turnHead(90);
}

void loop() {


};
