
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = A0; // analog pin connected to X output
const int Y_pin = A1; // analog pin connected to Y output
int16_t xPos;
int16_t yPos;
//int16_t swPos;
//int num = 1;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  xPos = analogRead(X_pin);
  yPos = analogRead(Y_pin);
//  swPos = digitalRead(SW_pin);

  int16_t data[] = {xPos, yPos};

  radio.write(&data, sizeof(data));
//  radio.write(&data[1], sizeof(data[1]));
//  radio.write(&data[2], sizeof(data[2]));
//  radio.write(&num, sizeof(num));

  delay(200);
}