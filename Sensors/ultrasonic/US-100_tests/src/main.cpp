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

#define ID "US-100" //<=== change the sensor name ====
#define enable_Ultrasonic
//#define enable_blinky

// General Library Definitions
#include <Arduino.h>

// US-100 Ultrasonic Sensor definitions
#ifdef enable_Ultrasonic
  #include <SoftwareSerial.h>           //Lib for the US-100 serial communication
  // Pins Assignments
  const int US100_TX = PA10;            //define the US100 tx pin
  const int US100_RX = PB3;             //define the US100 rx pin 

  SoftwareSerial US100Serial(US100_RX, US100_TX); //Pin definitions for RX, TX

  unsigned int MSB_byte_dist = 0;       //stores the MSB of distance value
  unsigned int LSB_byte_dist = 0;       //stores the LSB of distance value
  const float MIN_DISTANCE_CM = 10.0;   //Minimum distance to consider acceptable (cm)
  const float MAX_DISTANCE_CM = 100.0;  //Maximum distance to consider acceptable (cm)
  int num_readings_count = 0;                 //number of sensor readings
  int min_distance_counter = 0;         //count the number of sensor readings under minimum sensor range
  int max_distance_counter = 0;         //count the number of sensor readings over maximum sensor range
  float distance_moving_average = 0.0;  //stores the value of the distance moving average
  const int NUM_READINGS = 10;          //number of readings to take the moving avarage (média móvel)
  float readings[NUM_READINGS];         //array to store the readings
  int read_Index = 0;                   //index for the next array value
  float sum_of_total_readings = 0.0;    //stores the sum of the readings
  float distance_cm = 0.0;              //stores the distance_moving_average calculated distance value
  long reading_time = 0.0;              //time in ms
  long start_reading_interval = 60.0;   //time in ms
  long reading_interval = start_reading_interval;
  long stop_reading_interval = 5000.0;  //time in ms
  int reading_cycle = 1;                //counts the cycles of the readigs
  unsigned long previous_US_millis = 0; // Armazena o último instante em que o LED mudou de estado
  const int ledPin = LED_BUILTIN;       // Pino do LED (geralmente 13)
#endif //end enable_Ultrasonic

// General Variable definitions
const char* compilation_date = __DATE__;
const char* compilation_time = __TIME__;

#define SKETCH_DIR "\C:\\Projetos_GitHub\\PCAM_Performance_Tests\\Sensors\\ultrasonic\\US-100_tests\\src\\"
#define SKETCH_NAME "main.cpp"

#ifdef enable_blinky
  unsigned long previousMillis = 0; // Armazena o último instante em que o LED mudou de estado
  const long interval = 100; // Intervalo entre as piscadas (em milissegundos)
#endif

/**************************************** Headers of the Function definitions *****************************************/
void sketchSetup();
void ultrasonicSetup();
void ultrasonicRead();
void blinky();

/**************************************************** setup() ********************************************************/
void setup() {
    sketchSetup();
    #ifdef enable_Ultrasonic
      ultrasonicSetup();
    #endif

    pinMode(ledPin, OUTPUT); // sets ledPin as OUTPUT

} //end setup
 
/**************************************************** loop() *********************************************************/
void loop() {
  #ifdef enable_Ultrasonic
     ultrasonicRead();
  #endif
  #ifdef enable_blinky
    blinky();
  #endif
} //end loop


/**************************************************** Function definitions *******************************************/
/**************************************************** sketchSetup() **************************************************/
void sketchSetup() {
  // Show sketch initial description and starts serial monitor
  Serial.begin(115200); 

  Serial.println("\n\n******************************************************************************************"); 
  Serial.println("Starting Sensor: " + String(ID)); 
  Serial.println("\n");
  Serial.println("Compilation Date: " + String(__DATE__));
  Serial.println("Compilation Time: " + String(__TIME__));
  Serial.println("\nSystem version: US-100_20240727_01");
  Serial.println("");
  Serial.println("Developped by: Alexandre Nuernberg - alexandreberg@gmail.com");
  Serial.println("Master\'s thesis advisor: Sergio Augusto Bitencourt Petrovcic - sergio.petrovcic@ifsc.edu.br");
  Serial.println("");
  Serial.println("ProjectFolder: " + String(SKETCH_DIR));
  Serial.println("\nProject Sketch: " + String(SKETCH_NAME));
  Serial.println("\nThis project can be found in: https://github.com/alexandreberg/PCAM_Performance_Tests.git");
  Serial.println("******************************************************************************************"); 
  delay(1000);
}

/**************************************************** ultrasonicSetup() **********************************************/
void ultrasonicSetup(){
  Serial.println("\n\n\nTesting Ultrasonic Sensor US-100 on a Nucleo L476RG Board...\n\n");
  US100Serial.begin(9600); //US-100 is running in serial mode
  // initialize the array to store the readings with zeros
      for (int i = 0; i < NUM_READINGS; i++) {
        readings[i] = 0;
      } //end for
}

/**************************************************** ultrasonicRead() ***********************************************/
void ultrasonicRead(){
  unsigned long current_US_millis = millis();

  if (current_US_millis - previous_US_millis >= reading_interval && reading_time < stop_reading_interval) {  //start if millis
    // Checks if the time interval has elapsed
    previous_US_millis = current_US_millis;

    US100Serial.flush();
    US100Serial.write(0x55);  //trigger US100 to start measuring the distance
    delay(60);                //give the sensor time to make the measurement
  
      if(US100Serial.available() >= 2)    // check if received 2 bytes correctly
      {
          // Read the distance value
          // The distance value is stored in 2 bytes MSB_byte_dist and LSB_byte_dist
          MSB_byte_dist = US100Serial.read(); 
          LSB_byte_dist = US100Serial.read();

          num_readings_count++; //Counts the numer of sensor readings

          // To retrieve the distance, you have to do the following calculation:
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

            // Calculates the distance_moving_average
            distance_moving_average = sum_of_total_readings / NUM_READINGS;

            //Just start showing the distances after the distance moving average has being calculated
            if (num_readings_count >= NUM_READINGS) {
              Serial.println("distance_cm: " + String(distance_moving_average) + " cm | reading_interval: " + String(reading_interval) + " ms | reading_cycle: " + String(reading_cycle));
            }

          // Dobra o intervalo para a próxima leitura
          //reading_interval *= 2;
          //Each cycle increases the reading time by start_reading_interval ms
          reading_interval = start_reading_interval * reading_cycle;
          reading_cycle++;

          // Checks if the maximal reating time (stop_reading_interval) was reached
          if (reading_interval >= stop_reading_interval) {
            reading_interval = start_reading_interval;    //restarts the interval
            reading_cycle = 1;                            //restarts the counter
            Serial.println("Restarting the reading cycle");
          }

          } //end if
          else if (distance_cm <= MIN_DISTANCE_CM) {
            min_distance_counter++;
            num_readings_count = 0;
            // Serial.println("\n\nWARNING! - Reading outside acceptable range!");
            // Serial.println("Distance measured: " + String(distance_cm) + "cm, is shorter than MIN_DISTANCE_CM " + String(MIN_DISTANCE_CM) + "cm");
            // Serial.println("min_distance_counter = " + String(min_distance_counter) + "\n\n");
            Serial.print("distance_cm: " + String(distance_moving_average) + " cm | reading_interval: " + String(reading_interval) + " ms | reading_cycle: " + String(reading_cycle));
            Serial.println(" <=== WARNING MIN_DISTANCE_CM " + String (MIN_DISTANCE_CM) + " cm REACHED!");

          } //end else if 
          else if (distance_cm >= MAX_DISTANCE_CM) {
            max_distance_counter++;
            num_readings_count = 0;
            // Serial.println("\n\nWARNING! - Reading outside acceptable range!");
            // Serial.println("Distance measured: " + String(distance_cm) + "cm, is greather than MAX_DISTANCE_CM " + String(MAX_DISTANCE_CM) + "cm");
            // Serial.println("max_distance_counter = " + String(max_distance_counter) + "\n\n");
            Serial.print("distance_cm: " + String(distance_moving_average) + " cm | reading_interval: " + String(reading_interval) + " ms | reading_cycle: " + String(reading_cycle));
            Serial.println(" <=== WARNING MAX_DISTANCE_CM " + String (MAX_DISTANCE_CM) + " cm REACHED!");
            // TODO: distance_cm: 37.60 cm | reading_interval: 1080 ms | reading_cycle: 19 <=== WARNING MAX_DISTANCE_CM 100.00 cm REACHED!
            // distance_cm: 37.60 cm | reading_interval: 1080 ms | reading_cycle: 19 <=== WARNING MAX_DISTANCE_CM 100.00 cm REACHED!

          } //end else if 
      }

  } //end if millis
  else {
    //previous_US_millis = 0; //resets the reading_time do don't pass from stop_reading_interval
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
} //end function

/**************************************************** blinky() *******************************************************/
#ifdef enable_blinky
void blinky() {
  unsigned long currentMillis = millis(); // Obtém o tempo atual em milissegundos

  if (currentMillis - previousMillis >= interval) {
    // Verifica se já se passou o intervalo de tempo
    previousMillis = currentMillis; // Salva o instante atual

    // Inverte o estado do LED
    digitalWrite(ledPin, !digitalRead(ledPin));
  }
}
#endif