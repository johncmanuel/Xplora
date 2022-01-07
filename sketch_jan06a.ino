
#include <Servo.h> 

Servo servoLeft; // Declare left and right servos
Servo servoRight;


// Determines movement of the car
void movement(int timeLeft, int timeRight) {
  servoLeft.writeMicroseconds(timeLeft);
  servoRight.writeMicroseconds(timeRight);  
}

void setup() {

  servoLeft.attach(13);
  servoRight.attach(12);

  // Full speed backwards
  movement(1300, 1700);

  // Full speed forward
  movement(1700, 1300);
  
  delay(3000); // ...for 3 seconds
 
  servoLeft.detach(); // Stop sending servo signals
  servoRight.detach();

}

void loop() {
  
}
