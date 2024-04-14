#include <Arduino.h>

/*
  Blink with LowPower

  Turns an LED on for one second, then off for one second, repeatedly.

This code was changed to use the LowPower library to reduce power consumprion.
*/
#include <LowPower.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  
  /* The first argument, SLEEP_2S, puts the processor to sleep for two seconds. 
  To free up the processor, the PWM signal is created on an external clock, 
  which is disabled, turning off the ADC function saving additional power. 
  The Brown Out Timer (BOD), used to ensure the proper voltage is provided, 
  is also temporarily disabled.

  Sleep duration can be set to 15 ms, 30 ms, 60 ms, 120 ms, 250 ms, 500 ms, 1 Second, 
  2 seconds, 4 seconds, or 8 seconds. */
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);                    
  digitalWrite(LED_BUILTIN, LOW);    
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); 
}
