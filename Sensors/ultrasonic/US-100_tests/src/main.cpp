/*
Adjusting according with Sergio sugestions
date: 2024/24/07 

Testing the power consumption of US-100 (3V3) Ultrasonic Sensor.
The Power consumption of US-100 is being measured trough Nordic Power Profiler 
Kit II in Ameter mode (as amperimeter).

Testing the US-100 Ultrasonic Sensor in UART (Serial) Mode
Sensor operation: When the jumper is in place, you use 9600 baud UART to
communicate with the sensor. In UART mode, send 0x55 and read back two
bytes (16 bit value) that is mm distance, or 0x50 to read the temperature in °C. 

Board: STM32 Nucleo L476RG
************** Connections: ***************************
Nucleo 64   - Power_Profiler_Kit_II - US-100
3V3         - Vin (PPK)             - NC (Not Connected)
NC          - Vout (PPK)            - VCC (3V3)
PA10/D2     -                       - Trig/TX
PB3/D3      -                       - Echo/RX
GND         - GND                   - GND
GND         - GND                   - GND

Example of log results in serial monitor:
Testing Ultrasonic Sensor US-100 on a Nucleo L476RG Board...
distance_cm (filtered): 125 cm
distance_cm (filtered): 123 cm
distance_cm (filtered): 144 cm
distance_cm (filtered): 165 cm
distance_cm (filtered): 163 cm
distance_cm (filtered): 163 cm

*/

#define ID "Sensor_01" //<=== CHANGE HERE ====
#define enable_Ultrasonic

// General Library Definitions
#include <Arduino.h>

// US-100 Ultrasonic Sensor definitions
#ifdef enable_Ultrasonic
  #include <SoftwareSerial.h> //Lib for the US-100 serial communication
  // Pins Assignments
  const int US100_TX = PA10;  //define the US100 tx pin
  const int US100_RX = PB3;   //define the US100 rx pin 

  SoftwareSerial US100Serial(US100_RX, US100_TX); //RX, TX

  unsigned int MSB_byte_dist = 0;       //stores the MSB of distance value
  unsigned int LSB_byte_dist = 0;       //stores the LSB of distance value
  const float MIN_DISTANCE_CM = 10.0;   //Minimum distance to consider acceptable (cm)
  const float MAX_DISTANCE_CM = 100.0;  //Maximum distance to consider acceptable (cm)
  int min_distance_counter = 0;         //count the number of sensor readings under minimum sensor range
  int max_distance_counter = 0;         //count the number of sensor readings over maximum sensor range

  const int NUM_READINGS = 10;          //number of readings to take the moving avarage (média móvel)
  float readings[NUM_READINGS];         //array to store the readings
  int read_Index = 0;                   //index for the next array value
  float sum_of_total_readings = 0.0;    //stores the sum of the readings
  float readings_average = 0.0;         //stores the average of the readings
  float distance_cm = 0.0;              //stores the readings_average calculated distance value

#endif //end enable_Ultrasonic

// General Variable definitions
const char* compilation_date = __DATE__;
const char* compilation_time = __TIME__;

#define SKETCH_DIR "\C:\\Projetos_GitHub\\PCAM_Performance_Tests\\Sensors\\ultrasonic\\US-100_tests\\src\\"
#define SKETCH_NAME "main.cpp"

// Variables not used yet
int reading_interval = 10;      //time in ms
float moving_average = 0.0;     //stores the value of the moving readings_average
int test_time = 60 * 5;         // 5 minutes
int temp = 0;

/**************************************************** Headers of the Function definitions ********************************************************/
void sketchSetup();
void setupUltrasonic();
void readUltrasonic();


/**************************************************** setup() ********************************************************/
void setup() {
    sketchSetup();
    setupUltrasonic();
} //end setup
 
/**************************************************** loop() ********************************************************/
void loop() {
    readUltrasonic();
} //end loop


/**************************************************** Function definitions ********************************************************/
/**************************************************** sketchSetup() ********************************************************/
void sketchSetup() {
  // Show sketch initial description and starts serial monitor
  Serial.begin(115200); 

  Serial.println("Starting Sensor: " + String(ID)); 
  Serial.println("\nIlha 3d");
  Serial.println("\n(48) 99852-6523");
  Serial.println("\nwww.ilha3d.com");
  Serial.println("\n");
  Serial.println("Compilation Date: " + String(__DATE__));
  Serial.println("Compilation Time: " + String(__TIME__));
  Serial.println("\nSystem version: US-100_20240726_01");
  Serial.println("");
  Serial.println("Developer: Alexandre Nuernberg");
  Serial.println("Developers e-mail: alexandreberg@gmail.com");
  Serial.println("Master\'s thesis advisor: Sergio Augusto Bitencourt Petrovcic");
  Serial.println("Advisor\'s e-mail: sergio.petrovcic@ifsc.edu.br");
  Serial.println("");
  Serial.println("ProjectFolder: " + String(SKETCH_DIR));
  Serial.println("\nProject Sketch: " + String(SKETCH_NAME));
  Serial.println("This code can be found in: https://github.com/alexandreberg/PCAM_Performance_Tests.git");
  
  delay(1000);
}

/**************************************************** setupUltrasonic() ********************************************************/
void setupUltrasonic(){
  //Serial.println("Setting up US-100 Ultrasonic Sensor");
  Serial.println("\n\n\nTesting Ultrasonic Sensor US-100 on a Nucleo L476RG Board...\n\n");
  US100Serial.begin(9600); //US-100 is running in serial mode
  // initialize the array with zeros
      for (int i = 0; i < NUM_READINGS; i++) {
        readings[i] = 0;
      } //end for
}

/**************************************************** readUltrasonic() ********************************************************/
void readUltrasonic(){
  delay(5); //TODO see if it is needed
    US100Serial.flush();
    US100Serial.write(0x55); //command to start reading the distance
    delay(500);  //TODO see if it is needed
 
    if(US100Serial.available() >= 2) 
    {
        delay(10);
        // Read the distance value
        // The distance value is stored in 2 bytes MSB_byte_dist and LSB_byte_dist
        MSB_byte_dist = US100Serial.read(); 
        LSB_byte_dist = US100Serial.read();

        // To retrieve it you have to do the following calculation:
        distance_cm  = (MSB_byte_dist * 256 + LSB_byte_dist)/10; //divide by 10 to have the distance in cm otherwhise it is in mm

        // Filter the readings in the range of sensors specifications
        if (distance_cm > MIN_DISTANCE_CM && distance_cm < MAX_DISTANCE_CM) {
          // Removes the oldest reading from the sum_of_total_readings sum
          sum_of_total_readings -= readings[read_Index];

          // Store the new reading in the array
          readings[read_Index] = distance_cm;

          // Adds the new reading to the sum_of_total_readings sum
          sum_of_total_readings += readings[read_Index];

          // Advance to the next index in the array
          read_Index = (read_Index + 1) % NUM_READINGS;

          // Calculates the readings_average
          readings_average = sum_of_total_readings / NUM_READINGS;

          //TODO: Change the logic to just start showing the distances after the NUM_READINGS have being reached
          Serial.println("distance_cm (filtered): " + String(readings_average) + " cm");

        } //end if
        else if (distance_cm <= MIN_DISTANCE_CM) {
          min_distance_counter++;
          Serial.println("\n\nWARNING! - Reading outside acceptable range!");
          Serial.println("Distance measured: " + String(distance_cm) + "cm, is shorter than MIN_DISTANCE_CM " + String(MIN_DISTANCE_CM) + "cm");
          Serial.println("min_distance_counter = " + String(min_distance_counter) + "\n\n");

        } //end else if 
        else if (distance_cm >= MAX_DISTANCE_CM) {
          max_distance_counter++;
          Serial.println("\n\nWARNING! - Reading outside acceptable range!");
          Serial.println("Distance measured: " + String(distance_cm) + "cm, is greather than MAX_DISTANCE_CM " + String(MAX_DISTANCE_CM) + "cm");
          Serial.println("max_distance_counter = " + String(max_distance_counter) + "\n\n");
        } //end else if 

        delay(5000 - 5 - 500 - 10); // TODO: rethink the logic of the delays probably use millis(). Wait 5 seconds for new reading
      }
  //TODO: Reading Temperature: (it might be implemented to make distance corrections based on temperature)
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
}

