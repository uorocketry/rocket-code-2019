#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true
// this keeps track of whether we're using the interrupt
// off by default!
boolean usingInterrupt = false;

  typedef struct { //initialize structure
      double latitude;
      double longitude;
  } state;

state init_state;

void setup() {
  Serial.begin(115200);  // Start serial for output
  GPS.begin(9600); // Get sensor online


  typedef struct { //initialize structure
      double latitude;
      double longitude;
  } state;
  state init_state = {0.00, 0.00}; // set log, lat to 0
  delay(1000);
}

void loop() {

  update_state(&init_state); // function call pass memory address of init_state

  Serial.print("Latitude(Degrees):");
  Serial.print(init_state.latitude, 4);
  Serial.print("Longitude(Degrees):");
  Serial.print(init_state.longitude, 4); //2?
  Serial.println();
}

void update_state(state *state_ptr){ // point to memory address (get contents of mem address)
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'

      char c = GPS.read();
      // if you want to debug, this is a good time to do it!
      if (GPSECHO)
        if (c) Serial.print(c);

          // if a sentence is received, we can check the checksum, parse it...
      if (GPS.newNMEAreceived()) {
        // a tricky thing here is if we print the NMEA sentence, or data
        // we end up not listening and catching other sentences!
        // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
        //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

        if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
          return;  // we can fail to parse a sentence in which case we should just wait for another
      }
    state_ptr->latitude = GPS.latitude;
    state_ptr->longitude = GPS.longitude;

  }
}

void useInterrupt(boolean v) {
  if (v) {
    // Timer0 is already used for millis() - we'll just interrupt somewhere
    // in the middle and call the "Compare A" function above
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } else {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}
