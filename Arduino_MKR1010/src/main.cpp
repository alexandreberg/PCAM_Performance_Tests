/*
Mar/2024
Testing the US-100 Ultrasonic Sensor in UART (Serial) Mode
Sensor operation: When the jumper is in place, you use 9600 baud UART to
communicate with the sensor. In UART mode, send 0x55 and read back two
bytes (16 bit value) that is mm distance, or 0x50 to read the temperature in
degrees C. 

Board: STM32 Nucleo L476RG
Connections:
Nucleo 64   -   US-100
3V3         -   VCC
PA10/D2     -   Trig/TX
PB3/D3      -   Echo/RX
GND         -   GND
GND         -   GND


Tested ok, results in serial monitor:
Temp: 25 ºC.
Distance: 66 mm
Temp: 25 ºC.
Distance: 66 mm
*/
#include <Arduino.h>
//#include <SoftwareSerial.h>

#define US100Serial Serial1

//const int US100_TX = PIN_PA21;  //define the US100 tx pin
//const int US100_RX = PIN_PA20;   //define the US100 rx pin
 
//SoftwareSerial US100Serial(US100_RX, US100_TX); //RX, TX
unsigned int MSByteDist = 0;
unsigned int LSByteDist = 0;
unsigned int mmDist = 0;
int temp = 0;
const int probe7_Pin = PIN_PA21; //Logic analizer probe in D7 pin
const int probe6_Pin = PIN_PB11; //Logic analizer probe in D5 Pin

/*
const int probe5_Pin = D13; //Logic analizer probe 
const int probe4_Pin = D12; //Logic analizer probe 
const int probe3_Pin = D11; //Logic analizer probe 
const int probe2_Pin = D10; //Logic analizer probe 
*/
 
void setup() {
    //sensorSerial.begin(9600);
    Serial.begin(115200);
    US100Serial.begin(9600);
    pinMode(probe7_Pin, OUTPUT);
    pinMode(probe6_Pin, OUTPUT);
   /* 
    pinMode(probe5_Pin, OUTPUT);
    pinMode(probe4_Pin, OUTPUT);
    pinMode(probe3_Pin, OUTPUT);*/
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("Testing Ultrasonic Sensor US100 on an Arduino MKR WiFi 1010 Board");
}
 
void loop() {
    //probe7_Pin = Powerconsumption of LED_BUILTIN
    digitalWrite(probe7_Pin, HIGH);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(probe7_Pin, LOW);

    //probe6_Pin = Read distance
    digitalWrite(probe6_Pin, HIGH);
    delay(50);
    US100Serial.flush();
    US100Serial.write(0x55); //distance
    digitalWrite(probe6_Pin, LOW);
 
    delay(500);
 
    if(US100Serial.available() >= 2) 
    {
      //  digitalWrite(probe6_Pin, HIGH); //start probbing 
        delay(10);
        //delay(500);
        //Serial.println(String ("probe0: ") + digitalRead(probe0Pin));
        MSByteDist = US100Serial.read(); 
        LSByteDist = US100Serial.read();
        mmDist  = MSByteDist * 256 + LSByteDist; 
        if((mmDist > 1) && (mmDist < 10000)) 
        {
            Serial.print("Distance: ");
            Serial.print(mmDist, DEC);
            Serial.println(" mm");
        }
        //digitalWrite(probe6_Pin, LOW); //stop probbing 
    }
 
    //digitalWrite(probe5_Pin, HIGH);
    delay(10);
    US100Serial.flush(); 
    US100Serial.write(0x50); //temperature
    //digitalWrite(probe5_Pin, LOW);
 
    delay(500);
    if(US100Serial.available() >= 1) 
    {
      //  digitalWrite(probe4_Pin, HIGH); //start probbing 
        delay(10);
        //delay(500);
        temp = US100Serial.read();
        if((temp > 1) && (temp < 130)) // temprature is in range
        {
            temp -= 45; // correct 45º offset

            Serial.print("Temp: ");
            Serial.print(temp, DEC);
            Serial.println(" ºC.");
        //    digitalWrite(probe4_Pin, LOW); //stop probbing 
        }
    }
 
    delay(500);
}


