#include <Servo.h>

/*************************************************
* Public Constants (https://gist.github.com/mikeputnam/2820675)
* pitches.h
* These are the frequencies that the piezo buzzer can play at.
*************************************************/
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

// ========================================
// SOUND ==================================

// Use pin 4 for the piezo buzzer
const uint8_t buzzer = 4;

// Music notes for starting up the car
// Both arrays' sizes should equal the same
const uint16_t startUp[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4}; 
const uint8_t noteDur[] = {4, 8, 8, 4, 4, 4, 4, 4};

// Play sound from the piezo buzzer.
// Useful for indicating if the car is starting up, shutting down,
// low on batteries, etc.
void playSound(uint16_t hz, uint16_t ms) {
  
  // tone(pin, frequency, duration)
  tone(buzzer, hz, ms);
}

// Sound that plays when the car boots up.
void playStartUpSound() {
  for (uint8_t i = 0; i < (sizeof(startUp)/sizeof(startUp[0])); i++) {
    uint16_t dur = 1000 / noteDur[i];
    playSound(startUp[i], dur);
    
    uint16_t pause = dur * 1.30;
    delay(pause);
    noTone(buzzer);
  }
}

// ========================================
// MOVEMENT ===============================

// Set up servo motors
Servo servoLeft;
Servo servoRight;

// Move the car in whatever direction.
//
// Things to keep in mind for setting the parameters:
// 
// 1. (+)vals for moving robot forward
// 2. (-)vals for moving robot backward
// 3. 200       = full speed forward
// 4. -200      = full speed backward
// 5. 0         = stop
// 6. 100 to -100 = range for linear
// 7. -1      = disable servo signal
void maneuver(int16_t speedLeft, int16_t speedRight, int16_t ms) {
 
  int16_t sumLeft = 1500 + speedLeft;
  int16_t sumRight = 1500 - speedRight;
  
  // Rotate the servo motors
  servoLeft.writeMicroseconds(sumLeft);
  servoRight.writeMicroseconds(sumRight);
  
  // Disable servo signal
  if (ms == -1) {
    servoLeft.detach();
    servoRight.detach();
  }
  
  delay(ms);
  
  // Print the data
  Serial.print("Speed in the left direction: ");
  Serial.print(sumLeft);
  Serial.println(" microseconds");
  
  Serial.print("Speed in the right direction: ");
  Serial.print(sumRight);
  Serial.println(" microseconds");
  
  Serial.print("Total time: ");
  Serial.print(ms);
  Serial.println(" milliseconds");
}

void moveCarWithJoystick(int16_t x, int16_t y) {

     if (((x == 515 || x==514) && y == 521 || y == 520)) {
        maneuver(0,0,10);
        return;
     }
        // Forward
        if (y > 521) {
        maneuver(200, 200, 10);
        return;        // Backwards
        }else if (y < 521){
       maneuver(-200, -200, 10);
       return;
        }

       // Left
        if(x > 520){
        maneuver(-200, 200, 10);
        return;
        // Right
          }else if(x < 510){
        maneuver(200, -200, 10);
        return;
       }

       maneuver(0, 0, -1);  //disable servos
  
  }

// ========================================
// IR SENSOR ==============================

const uint8_t IRInputPin = 2;
const uint8_t IR_2_InputPin = 3;

// Assign names to IR sensors
const char* IRSensor = "IR Sensor Front: ";
const char* IR_2_Sensor = "IR Sensor Back: ";
                                       
int IROutputState, IR_2_OutputState;                                      
    
unsigned long startMillisIR;                            
unsigned long currentMillisIR;                          
const unsigned long periodIR = 200;

void checkObstacles(uint8_t pin, int* out, const char* sensorName) {

    startMillisIR = millis();                         
    
    if (currentMillisIR - startMillisIR >= periodIR){
        
        out = digitalRead(pin);
        Serial.print(sensorName);
              
        if (out == LOW) {
            Serial.println("There is obstacle !!!");    
          } else {                                             
            Serial.println("The path is clear");        
          }
          
        startMillisIR = millis();                       
      }
  }

// ========================================
// LIGHT SENSOR ===========================

// Define LED indications for light sensor
const uint8_t LED_1 = 5;
const uint8_t LED_2 = 6;

float volts(int adPin) {
  return float(analogRead(adPin)) * 5.0 / 1024.0;
}

// ========================================
// BLUETOOTH RECEIVER =====================

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const uint8_t CE_PIN = 7;
const uint8_t CNS_PIN = 8; 

RF24 radio(CE_PIN, CNS_PIN); // CE, CNS

const byte address[6] = "00001";

int16_t xPos, yPos, sw;

// ========================================
// MAIN FUNCTIONS =========================

void setup() {
  Serial.begin(9600);
  
  playStartUpSound();

  // Set up infrared sensor
  pinMode(IRInputPin, INPUT);                         
  startMillisIR = millis();

  // Set up phototransistor / light sensor
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);

  // Set up bluetooth receiver
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  servoLeft.attach(13);
  servoRight.attach(12);

//  maneuver(200, 200, 2000); //forward 2 seconds
//  maneuver(-200, -200, 2000); // backward 2 seconds
//  maneuver(-200, 200, 2000); // left 0.6 seconds
//  maneuver(200, -200, 2000); // right 0.6 seconds
//  maneuver(0, 0, -1);  //disable servos
  
}

void loop() {

  // Run bluetooth commmands
  if (radio.available()) {
      int16_t data[2];
      
      radio.read(&data, sizeof(data));
      // 0 - x
      // 1 - y
      Serial.print("X: ");
      Serial.println(data[0]);
      Serial.print("Y: ");
      Serial.println(data[1]);

      xPos = data[0];
      yPos = data[1];
      
  }
    moveCarWithJoystick(xPos, yPos);

  // Run infrared sensor commands
  checkObstacles(IRInputPin, IROutputState, IRSensor);
  checkObstacles(IR_2_InputPin, IR_2_OutputState, IR_2_Sensor);

  // Run phototransistor / light sensor commands
  Serial.print("A3 = ");
  Serial.print(volts(A3));
  Serial.println( " volts");

  delay(10);

  // Turn on LEDs if light levels are low
  if(volts(A3) <= 0.2) {
    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
  } else {
    digitalWrite(LED_1, LOW);
    digitalWrite(LED_2, LOW);
   }
}
