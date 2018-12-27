/*
 * 
 * Radar/Rangefinder
 * 
 */

#include <Ultrasonic.h>
#include "TM1637.h"

#define CLK 3
#define DIO 2

#define SPEAKER 4

TM1637 screen(CLK,DIO);
Ultrasonic ultrasonic(12, 13);

int distance;

void setup() {
  pinMode(SPEAKER, OUTPUT);
  screen.set(BRIGHT_DARKEST);
  screen.init(D4036B);
  screen.display(8888);
  delay(1500);
  
  Serial.begin(9600);
}

void beep() {
  digitalWrite(SPEAKER, HIGH);
  delay(5);
  digitalWrite(SPEAKER, LOW);
}

void loop() {
  distance = ultrasonic.read();

  screen.display(distance);

  beep();
  
  Serial.print("Distance in CM: ");
  Serial.println(distance);
  
  delay(1000);
}
