/*

 circuitdigest.com

 Sample STM32 Blink Program for Blue Pill board 

 */



// the setup function runs once when you press reset or power the board
#include <Arduino.h>
//int probe7 = PB12;
int probe6 = PB13;
int probe2 = PB14;
//int probe1 = PB15;

void setup() {

  // initialize digital pin PC13 as an output.

  pinMode(PC13, OUTPUT);
  //pinMode(probe7, OUTPUT);
  pinMode(probe6, OUTPUT);
  pinMode(probe2, OUTPUT);
  //pinMode(probe1, OUTPUT);
}


// the loop function runs over and over again forever

void loop() {

  //digitalWrite(probe7, HIGH);
  digitalWrite(probe2, HIGH);

  digitalWrite(PC13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(5000);              // wait for a second

  //digitalWrite(probe7, LOW);
  digitalWrite(probe2, LOW);
  digitalWrite(probe6, HIGH);
  //digitalWrite(probe1, HIGH);

  digitalWrite(PC13, LOW);    // turn the LED off by making the voltage LOW
  delay(5000);              // wait for a second

  digitalWrite(probe6, LOW);
  //digitalWrite(probe1, LOW);
}

