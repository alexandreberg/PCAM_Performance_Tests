/*  Testing the power consumption of HR-04 (5V version) Ultrasonic Sensor.
    The Power consumption of HR-04 is being measured trough Nordic Power Profiler Kit II in Ameter mode (as amperimeter).
    Schematics:

      UNO R3  - Power_Profiler_Kit_II - HR-04
      Vcc     - Vin (PPK) Vout        - Vcc
      12      -                       - TRIG
      13      -                       - ECHO
      GND     - GND                   - GND
    
    author: Alexandre Nuernberg alexandreberg@gmail.com
    date: may 2024

    This sketch takes 5 (NUM_READINGS) readings of HR-04 sensor stores it in an array if the readings are inside the acceptable 
    defined range (MIN_DISTANCE and MAX_DISTANCE).
    then it calculates the avarege of the NUM_READINGS and shows the resunt in the Serial Monitor
*/


#include <Arduino.h>
#define TRIGGER_PIN 12  // TRIGGER pin of HR-04
#define ECHO_PIN 13     // ECHO pin of HR-04

const int NUM_READINGS = 5; // number of readings to take the moving avarage (média móvel)
int readings[NUM_READINGS]; // Array to store the readings
int readIndex = 0;          // index for the next array value
long total = 0;             // sum of the readings
int average = 0;            // average of the readings

const int MIN_DISTANCE = 10;  // Minimum distance to consider acceptable (cm)
const int MAX_DISTANCE = 450; // Maximum distance to consider acceptable (cm)

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);

  // initialize the array with zeros
  for (int i = 0; i < NUM_READINGS; i++) {
    readings[i] = 0;
  }
}

void loop() {
  // start the trigger sending ultrasonic signal
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measures pulse return time
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculates distance in centimeters
  int distance = duration * 0.034 / 2;

  // Filter the readings
  if (distance >= MIN_DISTANCE && distance <= MAX_DISTANCE) {
    // Removes the oldest reading from the total sum
    total -= readings[readIndex];

    // Store the new reading in the array
    readings[readIndex] = distance;

    // Adds the new reading to the total sum
    total += readings[readIndex];

    // Advance to the next index in the array
    readIndex = (readIndex + 1) % NUM_READINGS;

    // Calculates the average
    average = total / NUM_READINGS;

    Serial.print("Distance (filtered): ");
    Serial.print(average);
    Serial.println(" cm");
  } else {
    Serial.println("Reading outside acceptable range");
  }

  delay(5000); // wait 5 seconds for new reading
}


