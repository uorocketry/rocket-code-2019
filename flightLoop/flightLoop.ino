#include <Adafruit_GPS.h>
#include "SparkFunMPL3115A2.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
#include "Yost.h"
#include <Wire.h>
#include <SD.h>
#include <Servo.h>

MPL3115A2 myPressure;
Adafruit_GPS myGPS(&mySerial);
const int fromLow = 0;
const int fromHigh = 10;
const int toLow = 170;
const int toHigh = 180;
int percent;
Servo myservo;
int pos = 0;
String value_log_transmit;
int counter=0;
typedef struct { //initialize structure
  float altitude;
  float velocity;
  float latitude;
  float longitude;
} state;

void setup() {

  Wire.begin();       // Join i2c bus
  myPressure.begin(9600);
  myGPS.begin(9600);
  yost.begin();
  myservo.attach(6);
  Serial.begin(57600);
}

void loop() {
  readSensorts();
  deployBrakes();
  logValues();
  update_state(state *state_ptr);
}

void update_state(state *state_ptr){ // point to memory address (get contents of mem address)

  state_ptr->altitude = myPressure.readAltitude(); // the altitude field in the struct that state_ptr points to is updated to current Altitude
  state_ptr->latitude = myGPS.latitude;
  state_ptr->longitude = myGPS.longitude;


  // yost imu library
    float *euler_orient = yost.read_orientation_euler(); // point to memory address of array

    state_ptr->rocket.e_orient.pitch = *euler_orient; // point (imu_ptr) to e_orient and access the pitch filed
    state_ptr->rocket.e_orient.yaw = *(euler_orient + 1);
    state_ptr->rocket.e_orient.roll = *(euler_orient + 2);

    // yost imu library
    float *accel = yost.read_accel_filtered(); // point to memory address of array

    state_ptr->rocket.r_accel.x = *accel; // point (imu_ptr) to r_accel and access the x filed
    state_ptr->rocket.r_accel.y = *(accel + 1);
    state_ptr->rocket.r_accel.z = *(accel + 2);

}

void deployBrakes() {

  int percent=function2(velocity, altitude);
  percent=map(percent, fromLow, fromHigh, toLow, toHigh);

  for (pos = 0; pos <= percent; pos+=1) { //goes from 0 degrees to the percentage from the lookup table
    myservo.write(pos);                   // tell servo to go to position in variable 'pos'
    delay(15);                            // waits 15ms for the servo to reach the position
  }

  for (pos=percent; pos>=0; pos-=1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);               // tell servo to go to position in variable 'pos'
    delay(15);                        // waits 15ms for the servo to reach the position
  }
}

void logValues() {

  state init_state;
  counter++;
  store_state(state init_state);
  values_log_transmit = serialize_state(state init_state);
  sdcard_file = SD.open("datalog.txt", FILE_WRITE);

  if (sdcard_file) {
    if (counter%10==0) {
      Serial.write(values_log_transmit);
      Serial.write("\r\n");
    } else {
      sdcard_file.println(values_log_transmit);
    }
  } else {
    Serial.println("Error opening datalog.txt");
  }
}
