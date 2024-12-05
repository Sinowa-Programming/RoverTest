// RoverTest.ino
// FGCU Computing & Software Engineering
// Prof. Allen
// Test the basic Rover robot interfaces. Uses a set of states
// to run the wheels through each of the basic movements and
// adjusts the head to point in the direction of movement. The
// Serial inteface is used to output direction of movement, along
// with direction of the head and distance measured.
// ------------------------------------------------------------

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

fgcu::FourPin motorPinsLeft { A0, A1, A2, A3};
fgcu::FourPin motorPinsRight { 4, 5, 6, 7};
float speed = 600.f;
fgcu::RoverWheels wheels{motorPinsLeft, motorPinsRight, speed};

RoverWheelState wheelStates[] = { Straight, TurnLeft, ShortStraight, Backup, 
                                  CorrectRight, ShortStraight, CorrectRight, 
                                  Straight, Stop };
int wheelStateCount = 9;
int wheelStateIndex = 0;

const byte ServoPin = 9;
const byte EchoPin = A5;
const byte TriggerPin = A4;

fgcu::RoverHead head{EchoPin, TriggerPin, ServoPin};



void setup() {
  delay(2000);
  head.turnHead(90);

  // blocking call to get the head turned 
  // to 90 degrees and distance taken
  bool done = false;
  do {
    done = head.run();
  } while (!done);
  
} // setup

const int thresholdFront = 10; // Distance to trigger turn
const int thresholdSide = 10;  // Distance to trigger correction
 //read distances  
  word distFront=0, distLeft=0, distRight=0;

void loop() {
  wheels.run();

  
  if(!wheels.isMoving()){
    // Measure forward distance
    head.turnHead(99); // Center
    while (head.isMeasuring()) {
      head.run();
    }

    distFront = head.getDistance();

    // Measure left distance
    head.turnHead(190); // Turn head left
    while (head.isMeasuring()) {
      head.run();
    }
    distLeft = head.getDistance();

    // Measure right distance
    head.turnHead(0); // Turn head right
    while (head.isMeasuring()) {
      head.run();
    }
    distRight = head.getDistance();

    if (distFront > thresholdFront) {
    // wheels.run();
      wheels.moveForward();
    }
    else if (distLeft < distRight) {
      // wheels.run();
      wheels.turnLeft();
      // wheels.moveForward();
    } else {
    // wheels.run();
      wheels.turnRight();
    // wheels.moveForward();      
    }
  }
}

// method uses the states collection to see what wheel state to execute next
void moveWheels() {

  switch (wheelStates[wheelStateIndex]) {
    case Stop:
      Serial.print("Stop\n");
      wheels.stop();
      head.turnHead(90);
      break;
    case Straight:
      Serial.print("Straight\n");
      wheels.moveForward();
      head.turnHead(90);
      break;
    case ShortStraight:
      Serial.print("ShortStraight\n");
      wheels.moveForward(0.5f);
      head.turnHead(90);
      break;
    case CorrectLeft:
      Serial.print("CorrectLeft\n");
      wheels.turnLeft(0.5f);
      head.turnHead(135);
      break;
    case TurnLeft:
      Serial.print("TurnLeft\n");
      wheels.turnLeft();
      head.turnHead(180);
      break;
    case CorrectRight:
      Serial.print("CorrectRight\n");
      wheels.turnRight(0.5f);
      head.turnHead(45);
      break;
    case TurnRight:
      Serial.print("TurnRight\n");
      wheels.turnRight();
      head.turnHead(0);
      break;
    case Backup:
      Serial.print("Backup\n");
      wheels.moveBackward();
      head.turnHead(90);
  } // rover state

} // moveWheels

