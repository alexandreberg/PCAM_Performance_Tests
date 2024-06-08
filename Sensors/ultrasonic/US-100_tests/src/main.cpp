/*
Testing the power consumption of US-100 (3V3) Ultrasonic Sensor.
The Power consumption of US-100 is being measured trough Nordic Power Profiler 
Kit II in Ameter mode (as amperimeter).

Testing the US-100 Ultrasonic Sensor in UART (Serial) Mode
Sensor operation: When the jumper is in place, you use 9600 baud UART to
communicate with the sensor. In UART mode, send 0x55 and read back two
bytes (16 bit value) that is mm distance, or 0x50 to read the temperature in
degrees C. 

Board: STM32 Nucleo L476RG
Connections:
Nucleo 64   - Power_Profiler_Kit_II - US-100
3V3         - Vin (PPK) Vout        - VCC
PA10/D2     -                       - Trig/TX
PB3/D3      -                       - Echo/RX
GND         - GND                   - GND
GND         - GND                   - GND

Log resultns in serial monitor:

Testing Ultrasonic Sensor US-100 on a Nucleo L476RG Board...
distance_cm (filtered): 125 cm
distance_cm (filtered): 123 cm
distance_cm (filtered): 144 cm
distance_cm (filtered): 165 cm
distance_cm (filtered): 163 cm
distance_cm (filtered): 163 cm

*/
#include <Arduino.h>
#include <SoftwareSerial.h>

const int US100_TX = PA10;  //define the US100 tx pin
const int US100_RX = PB3;   //define the US100 rx pin 

SoftwareSerial US100Serial(US100_RX, US100_TX); //RX, TX

unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int distance_cm = 0;
int temp = 0;

const int NUM_READINGS = 5; // number of readings to take the moving avarage (média móvel)
int readings[NUM_READINGS]; // Array to store the readings
int readIndex = 0;          // index for the next array value
long total = 0;             // sum of the readings
int average = 0;            // average of the readings

const int MIN_DISTANCE_CM = 10;  // Minimum distance to consider acceptable (cm)
const int MAX_DISTANCE_CM = 450; // Maximum distance to consider acceptable (cm)
 
void setup() {
    Serial.begin(115200);
    US100Serial.begin(9600);
    Serial.println("Testing Ultrasonic Sensor US-100 on a Nucleo L476RG Board...");

    // initialize the array with zeros
    for (int i = 0; i < NUM_READINGS; i++) {
      readings[i] = 0;
    } //end for
} //end setup
 
void loop() {
    delay(5);
    US100Serial.flush();
    US100Serial.write(0x55); //distance
    delay(500);
 
    if(US100Serial.available() >= 2) 
    {
        delay(10);
        MSByteDist = US100Serial.read(); 
        LSByteDist = US100Serial.read();
        distance_cm  = (MSByteDist * 256 + LSByteDist)/10; 

        // Filter the readings
        if (distance_cm >= MIN_DISTANCE_CM && distance_cm <= MAX_DISTANCE_CM) {
          // Removes the oldest reading from the total sum
          total -= readings[readIndex];

          // Store the new reading in the array
          readings[readIndex] = distance_cm;

          // Adds the new reading to the total sum
          total += readings[readIndex];

          // Advance to the next index in the array
          readIndex = (readIndex + 1) % NUM_READINGS;

          // Calculates the average
          average = total / NUM_READINGS;

          Serial.print("distance_cm (filtered): ");
          Serial.print(average);
          Serial.println(" cm");
        } else {
          Serial.println("Reading outside acceptable range");
        }

        delay(5000 - 5 - 500 - 10); // wait 5 seconds for new reading
      }

 

 //Reading Temperature:
 /*
    delay(10);
    US100Serial.flush(); 
    US100Serial.write(0x50); //temperature
 
    delay(500);
    if(US100Serial.available() >= 1) 
    {
        delay(10);
        temp = US100Serial.read();
        if((temp > 1) && (temp < 130)) // temprature is in range
        {
            temp -= 45; // correct 45º offset

            Serial.print("Temp: ");
            Serial.print(temp, DEC);
            Serial.println(" ºC.");
        } //end internal if
    } //end external if
 
    delay(500);
  */
} //end loop


