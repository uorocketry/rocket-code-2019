#include <Wire.h>
#include <Adafruit_GPS.h>
#include <stdio.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
Adafruit_GPS myGPS(&mySerial);

  typedef struct { //initialize structure
      float latitude;
      float longitude; 
  } state;
  
void setup() {       
  Serial.begin(9600);  // Start serial for output
  myGPS.begin(9600); // Get sensor online

  
  typedef struct { //initialize structure
      float latitude;
      float longitude; 
  } state;
  state init_state = {myGPS.latitude, myGPS.longitude}; // random velocity picked

}

void loop() {
  state init_state;
  update_state(&init_state); // function call pass memory address of init_state
  
  Serial.print("Latitude(Degrees):");
  Serial.print(init_state.latitude, 2);
  Serial.print("Longitude(Degrees):");
  Serial.print(init_state.longitude, 2); //2?
  Serial.println();
}

void update_state(state *state_ptr){ // point to memory address (get contents of mem address)
  state_ptr->latitude = myGPS.latitude; 
  state_ptr->longitude = myGPS.longitude; 
}
