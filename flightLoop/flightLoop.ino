/******************************************************************************
                              general includes
*******************************************************************************/
#include <EEPROM.h>
#include <Wire.h> //i2c

/******************************************************************************
                              altimeter includes
*******************************************************************************/
#include <SparkFunMPL3115A2.h>

/******************************************************************************
                              GPS includes
*******************************************************************************/
#include <Adafruit_GPS.h>
#include <stdio.h>
#include <SoftwareSerial.h>

/******************************************************************************
                              YOST includes
*******************************************************************************/
#include "Yost.h"

/******************************************************************************
                              Servo includes
*******************************************************************************/
#include <Servo.h>

/******************************************************************************
                              SD card includes
*******************************************************************************/
#include <SdFat.h>

/******************************************************************************
                              general variables
*******************************************************************************/
boolean sensor_debug = false;

typedef struct { //Euler angle struct (YOST)
  float pitch;
  float yaw;
  float roll;
} euler_angles;

typedef struct { //Acceleration Struct (YOST)
  float x;
  float y;
  float z;
} accel;

typedef struct { //YOST struct (belongs to State)
  euler_angles e_orient;
  accel r_accel;
} yost_imu; //

typedef struct { //overall state struct
  float altitude;
  float velocity; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
  double latitude;
  double longitude;
  yost_imu rocket;
} state;

String values_log_transmit; //string for logging and radio transmission
int counter = 0; //counter for how many times the loop function runs

euler_angles init_euler_angles;
accel init_accel;
yost_imu init_yost_imu;
state init_state;    //Initialize struct

double prev_alt=0;
int time;
int temp_time;
/******************************************************************************
                              altimeter variables
*******************************************************************************/
MPL3115A2 myPressure;

/******************************************************************************
                              GPS variables
*******************************************************************************/
SoftwareSerial mySerial(3, 2);
Adafruit_GPS GPS(&mySerial);
#define GPSECHO  true
boolean usingInterrupt = false;

/******************************************************************************
                              YOST variables
*******************************************************************************/
Yost yost;

/******************************************************************************
                              SD card variables
*******************************************************************************/
const int chipSelect = 4;
SdFat sd;
SdFile myFile;
char fileName[] = "log.txt";

/******************************************************************************
                              Servo variables
*******************************************************************************/
const int fromLow = 0; //these four variables are for the map() function
const int fromHigh = 100;
const int toLow = 553;
const int toHigh = 2800;
int percent;
Servo myservo;
int pos = map(0, fromLow, fromHigh, toLow, toHigh);

void setup() {
  Wire.begin();        // Join i2c bus
  myPressure.begin();  //Initialize Altimeter
  myPressure.setModeAltimeter();
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
  GPS.begin(9600);   //Initialize GPS
  yost.begin();        //Initialize IMU
  myservo.attach(6);   //Initialize Air Brake
  Serial.begin(115200); //Start serial for radio
  time = millis();
  temp_time = time;
  euler_angles init_euler_angles = {0.00, 0.00, 0.00};
  accel init_accel = {0.00, 0.00, 0.00};
  yost_imu init_yost_imu = {init_euler_angles, init_accel};
  state init_state = {myPressure.readAltitude(), 0.00, GPS.latitude, GPS.longitude, init_yost_imu};
  delay(2000);
}

void loop() {
  if(sensor_debug){
    read_dummy_sensors(&init_state);
  } else {
    //time = millis();
    //temp_time = time;
    update_state(&init_state); // function call pass memory address of init_state
    //time = millis();
    //Serial.println("it took " + String(time - temp_time) + " to run update_state");
  }
  //time = millis();
  //temp_time = time;
  deployBrakes(&init_state);
  //time = millis();
  //Serial.println("it took " + String(time - temp_time) + " to run deployBrakes");

  //time = millis();
  //temp_time = time;
  logValues(&init_state);
  //time = millis();
  //Serial.println("it took " + String(time - temp_time) + " to run logValues");
}

void update_state(state *state_ptr) { // point to memory address (get contents of mem address)
  state_ptr->altitude = myPressure.readAltitude();
  if (! usingInterrupt) {
    // read data from the GPS in the 'main loop'
      char c = GPS.read();
      // if you want to debug, this is a good time to do it!
      if (GPSECHO)
        if (c) //Serial.print(c);
          // if a sentence is received, we can check the checksum, parse it...
      if (GPS.newNMEAreceived()) {
        if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
          return;
      }
      state_ptr->latitude = GPS.latitude;
      state_ptr->longitude = GPS.longitude;
  }

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

void deployBrakes(state *state_ptr) {
  int percent = lookUpBrakes(state_ptr);
  percent = map(percent, fromLow, fromHigh, toLow, toHigh);
  if(pos > percent){
    for (pos = percent; pos >= percent; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);               // tell servo to go to position in variable 'pos'
      delay(3);                        // waits 15ms for the servo to reach the position
    }
  } else {
    for (pos ; pos <= percent; pos += 1) { //goes from 0 degrees to the percentage from the lookup table
      myservo.write(pos);                   // tell servo to go to position in variable 'pos'
      delay(3);                            // waits 15ms for the servo to reach the position
    }
  }
  pos = percent;

}

void logValues(state *state_ptr) {

  //Serial.println(counter); //TODO: delete me
  //values_log_transmit = serialize_state(state_ptr);
  //Serial.println(values_log_transmit);

  if (counter % 10 == 0) {
    //Serial.println(values_log_transmit);
    //serialize_state(state_ptr, true);
    serialize_state(true);

    //Serial.write("\r\n");
  }
  sd.begin(chipSelect, SPI_HALF_SPEED);
  myFile.open(fileName, O_RDWR | O_CREAT | O_AT_END);
  //myFile.println(values_log_transmit);
  //serialize_state(state_ptr, false);
  serialize_state(false);

  //delay(100);
  myFile.close();

  counter++;
}


//void serialize_state(state *state_ptr, boolean radio) {
void serialize_state(boolean radio) {
   if(radio){
    Serial.print(millis());
    Serial.print(" ");
    Serial.print(init_state.altitude);
    Serial.print(" ");
    Serial.print(init_state.latitude);
    Serial.print(" ");
    Serial.print(init_state.longitude);
    Serial.print(" ");
    Serial.print(init_state.rocket.e_orient.pitch);
    Serial.print(" ");
    Serial.print(init_state.rocket.e_orient.yaw);
    Serial.print(" ");
    Serial.print(init_state.rocket.e_orient.roll);
    Serial.print(" ");
    Serial.print(init_state.rocket.r_accel.x);
    Serial.print(" ");
    Serial.print(init_state.rocket.r_accel.y);
    Serial.print(" ");
    Serial.println(init_state.rocket.r_accel.z);

  } else {
    myFile.print(millis());
    myFile.print(" ");
    myFile.print(init_state.altitude);
    myFile.print(" ");
    myFile.print(init_state.latitude);
    myFile.print(" ");
    myFile.print(init_state.longitude);
    myFile.print(" ");
    myFile.print(init_state.rocket.e_orient.pitch);
    myFile.print(" ");
    myFile.print(init_state.rocket.e_orient.yaw);
    myFile.print(" ");
    myFile.print(init_state.rocket.e_orient.roll);
    myFile.print(" ");
    myFile.print(init_state.rocket.r_accel.x);
    myFile.print(" ");
    myFile.print(init_state.rocket.r_accel.y);
    myFile.print(" ");
    myFile.println(init_state.rocket.r_accel.z);
  }
}

int lookUpBrakes(state *state_ptr) {

  unsigned int first_four_bits; // first four bits of the address
  //switch case sets first four bits of address based off the velocity
  switch (int(state_ptr->velocity)) {
    case 0 ... 49:
      first_four_bits = 0b0000;
      break;

    case 50 ... 63:
      first_four_bits = 0b0001;
      break;

    case 64 ... 77:
      first_four_bits = 0b0010;
      break;

    case 78 ... 91:
      first_four_bits = 0b0011;
      break;

    case 92 ... 105:
      first_four_bits = 0b0100;
      break;

    case 106 ... 119:
      first_four_bits = 0b0101;
      break;

    case 120 ... 133:
      first_four_bits = 0b0110;
      break;

    case 134 ... 147:
      first_four_bits = 0b0111;
      break;

    case 148 ... 161:
      first_four_bits = 0b1000;
      break;

    case 162 ... 175:
      first_four_bits = 0b1001;
      break;

    case 176 ... 189:
      first_four_bits = 0b1010;
      break;

    case 190 ... 203:
      first_four_bits = 0b1011;
      break;

    case 204 ... 217:
      first_four_bits = 0b1100;
      break;

    case 218 ... 231:
      first_four_bits = 0b1101;
      break;

    case 232 ... 245:
      first_four_bits = 0b1110;
      break;

    default:
      first_four_bits = 0b1111;
      break;
  }
  //switch case last five bits of address based off the altitude
  unsigned int last_five_bits;
  switch (int(state_ptr->altitude)) {
    case 0 ... 99:
      last_five_bits = 0b00000;
      break;

    case 100 ... 199:
      last_five_bits = 0b00001;
      break;

    case 200 ... 299:
      last_five_bits = 0b00010;
      break;

    case 300 ... 399:
      last_five_bits = 0b00011;
      break;

    case 400 ... 499:
      last_five_bits = 0b00100;
      break;

    case 500 ... 599:
      last_five_bits = 0b00101;
      break;

    case 600 ... 699:
      last_five_bits = 0b00110;
      break;

    case 700 ... 799:
      last_five_bits = 0b00111;
      break;

    case 800 ... 899:
      last_five_bits = 0b01000;
      break;

    case 900 ... 999:
      last_five_bits = 0b01001;
      break;

    case 1000 ... 1099:
      last_five_bits = 0b01010;
      break;

    case 1100 ... 1199:
      last_five_bits = 0b01011;
      break;

    case 1200 ... 1299:
      last_five_bits = 0b01100;
      break;

    case 1300 ... 1399:
      last_five_bits = 0b01101;
      break;

    case 1400 ... 1499:
      last_five_bits = 0b01110;
      break;

    case 1500 ... 1599:
      last_five_bits = 0b01111;
      break;

    case 1600 ... 1699:
      last_five_bits = 0b10000;
      break;

    case 1700 ... 1799:
      last_five_bits = 0b10001;
      break;

    case 1800 ... 1899:
      last_five_bits = 0b10010;
      break;

    case 1900 ... 1999:
      last_five_bits = 0b10011;
      break;

    case 2000 ... 2099:
      last_five_bits = 0b10100;
      break;

    case 2100 ... 2199:
      last_five_bits = 0b10101;
      break;

    case 2200 ... 2299:
      last_five_bits = 0b10110;
      break;

    case 2300 ... 2399:
      last_five_bits = 0b10111;
      break;

    case 2400 ... 2499:
      last_five_bits = 0b11000;
      break;

    case 2500 ... 2599:
      last_five_bits = 0b11001;
      break;

    case 2600 ... 2600:
      last_five_bits = 0b11010;
      break;

    case 2700 ... 2799:
      last_five_bits = 0b11011;
      break;

    case 2800 ... 2899:
      last_five_bits = 0b11100;
      break;

    case 2900 ... 2999:
      last_five_bits = 0b11101;
      break;

    case 3000 ... 3100:
      last_five_bits = 0b11110;
      break;

    default:
      last_five_bits = 0b11111;
      break;
  }
  //x is address
  int x = first_four_bits * 16 + (last_five_bits / 2); // concatanates both adress components arithmetically
  int b = EEPROM.read(x); // gets EEPROM value at address x(8 bit int)
  int percent; // airbreak percent
  if (last_five_bits % 2 == 0) { // if last 9th bit is 0 it uses the first 4 bits of EEPROM value at x
    percent = b / 16; // divides by 16 getting rid of the last 4 bits, only leaving the first 4 bits
  }
  else {
    percent = b - (b / 16) * 16; // gets last 4 bits
  }
  return percent; // return percent

}

double getVelocity(float altitude) {
  double velocity = (altitude - prev_alt)/((time-temp_time)*1000);
  temp_time = time;
  time = millis();
  prev_alt = altitude;
  return velocity;

}

void read_dummy_sensors(state *state_ptr) {
  float *euler_orient = yost.read_orientation_euler();
  float *accel = yost.read_accel_filtered();
  while(!Serial.available()){
  }

  if(Serial.available()>0) {
    String data_from_python = Serial.readString();
    int commaIndex = data_from_python.indexOf(",");
    int secondCommaIndex = data_from_python.indexOf(",", commaIndex+1);
    int thirdCommaIndex = data_from_python.indexOf(",", secondCommaIndex+1);
    int fourthCommaIndex = data_from_python.indexOf(",", thirdCommaIndex+1);
    int fifthCommaIndex = data_from_python.indexOf(",", fourthCommaIndex+1);
    int sixthCommaIndex = data_from_python.indexOf(",", fifthCommaIndex+1);
    int seventhCommaIndex = data_from_python.indexOf(",", sixthCommaIndex+1);
    int eighthCommaIndex = data_from_python.indexOf(",", seventhCommaIndex+1);
    int ninthCommaIndex = data_from_python.indexOf(",", eighthCommaIndex+1);


    String firstValue = data_from_python.substring(0, commaIndex);
    String secondValue = data_from_python.substring(commaIndex+1, secondCommaIndex);
    String thirdValue = data_from_python.substring(secondCommaIndex+1, thirdCommaIndex);
    String fourthValue = data_from_python.substring(thirdCommaIndex+1, fourthCommaIndex);
    String fifthValue = data_from_python.substring(fourthCommaIndex+1, fifthCommaIndex);
    String sixthValue = data_from_python.substring(fifthCommaIndex+1, sixthCommaIndex);
    String seventhValue = data_from_python.substring(sixthCommaIndex+1, seventhCommaIndex);
    String eighthValue = data_from_python.substring(seventhCommaIndex+1, eighthCommaIndex);
    String ninthValue = data_from_python.substring(eighthCommaIndex+1, ninthCommaIndex);
    String tenthValue = data_from_python.substring(ninthCommaIndex+1);


    state_ptr->altitude = firstValue.toFloat();
    state_ptr->velocity = secondValue.toFloat();
    state_ptr->latitude = thirdValue.toFloat();
    state_ptr->longitude = fourthValue.toFloat();

    state_ptr->rocket.e_orient.pitch = fifthValue.toFloat();
    state_ptr->rocket.e_orient.yaw = sixthValue.toFloat();
    state_ptr->rocket.e_orient.roll = seventhValue.toFloat();

    state_ptr->rocket.r_accel.x = eighthValue.toFloat();
    state_ptr->rocket.r_accel.y = ninthValue.toFloat();
    state_ptr->rocket.r_accel.z = tenthValue.toFloat();
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
