#include <Wire.h>
#include "SparkFunMPL3115A2.h"
#include <stdio.h>
#include <SdFat.h>
const int chipSelect = 4;
SdFat sd;
SdFile myFile;

char fileName[] = "log.txt";
int sdPower = LED_BUILTIN;
MPL3115A2 myPressure;

  typedef struct { //initialize structure
      float altitude;
      float velocity; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
  } state;

void setup() {
  pinMode(sdPower,OUTPUT);
  Wire.begin();        // Join i2c bus
  Serial.begin(9600);  // Start serial for output
  myPressure.begin(); // Get sensor online

  //Configure the sensor
  myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
  //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags

  typedef struct { //initialize structure
      float altitude;
      float velocity; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
  } state;
  state init_state = {myPressure.readAltitude(), 20.0}; // random velocity picked

// function prototype
void update_state(state *); // pointer to struct
}

void loop() {
  state init_state;
  update_state(&init_state); // function call pass memory address of init_state

  Serial.print("Altitude(m):");
  Serial.print(init_state.altitude, 2);
  Serial.println();
  store_state(init_state);
}

void update_state(state *state_ptr){ // point to memory address (get contents of mem address)
  state_ptr->altitude = myPressure.readAltitude(); // the altitude field in the struct that state_ptr points to is updated to current Altitude
}

void store_state(state init_state) {
  //digitalWrite(sdPower,HIGH);
  sd.begin(chipSelect, SPI_HALF_SPEED);
  myFile.open(fileName, O_RDWR | O_CREAT | O_AT_END);

  myFile.println("Altitude(m): "+serialize_state(init_state));

  //delay(1000);
  myFile.close();

  //digitalWrite(sdPower,LOW);
  //delay(100);
}

String serialize_state(state init_state) {
  return String(init_state.altitude);
}
