#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
#define GPSECHO  true
Adafruit_GPS myGPS(&mySerial);
boolean sensor_debug = false;

typedef struct { //initialize structure
      float latitude;
      float longitude; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
  } state;

void setup() {
  
  Serial.begin(9600); //Start serial for output
  myGPS.begin(9600); //Get sensor online
  

}

void loop() {
  
  read_GPS();

}

void read_GPS() {
  
  if (!sensor_debug) {
    
    char c = myGPS.read();
    // if you want to debug, this is a good time to do it!
    
    if (GPSECHO)
      if (c) Serial.print(c);

    if (myGPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences! 
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false
  
    if (!myGPS.parse(myGPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
    }
    
    Serial.print("Location: ");
      Serial.print(myGPS.latitude, 4); Serial.print(myGPS.lat);
      Serial.print(", "); 
      Serial.print(myGPS.longitude, 4); Serial.println(myGPS.lon);
      Serial.print("Location (in degrees, works with Google Maps): ");
      Serial.print(myGPS.latitudeDegrees, 4);
      Serial.print(", "); 
      Serial.println(myGPS.longitudeDegrees, 4);

    Serial.println();
  } else {
    if (Serial.available() > 0) {
      int x = Serial.read();
      Serial.write(x);
    }
  }
  
}
