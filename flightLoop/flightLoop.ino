#include <SparkFunMPL3115A2.h>
#include <EEPROM.h>
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 2);
#include "Yost.h"
#include <Wire.h>
#include <Servo.h>
Yost yost;
#include <SdFat.h>
const int chipSelect = 4;
SdFat sd;
SdFile myFile;
char fileName[] = "log.txt";
int sdPower = LED_BUILTIN;

MPL3115A2 myPressure;
Adafruit_GPS myGPS(&mySerial);
const int fromLow = 0;
const int fromHigh = 10;
const int toLow = 170;
const int toHigh = 180;
int percent;
Servo myservo;
int pos = 0;
String values_log_transmit;
int counter = 0;

typedef struct { //initialize struct
  float pitch;
  float yaw;
  float roll;
} euler_angles;

typedef struct {
  float x;
  float y;
  float z;
} accel;

typedef struct {
  euler_angles e_orient;
  accel r_accel;
} yost_imu; // user defined data type

typedef struct { //initialize structure
  float altitude;
  float velocity; // velocity to be implemented later (a_1 - a_0) / time_inbetween_readings
  float latitude;
  float longitude;
  yost_imu rocket;
} state;

void setup() {

  Wire.begin();       // Join i2c bus
  myPressure.begin();
  myGPS.begin(9600);
  yost.begin();
  myservo.attach(6);
  Serial.begin(57600);
  pinMode(sdPower,OUTPUT);



}

void loop() {
  state init_state;
  update_state(&init_state); // function call pass memory address of init_state
  deployBrakes(&init_state);
  logValues(init_state);
}

void update_state(state *state_ptr) { // point to memory address (get contents of mem address)

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

void deployBrakes(state *state_ptr) {

  int percent = lookUpBrakes(state_ptr);
  percent = map(percent, fromLow, fromHigh, toLow, toHigh);

  for (pos = 0; pos <= percent; pos += 1) { //goes from 0 degrees to the percentage from the lookup table
    myservo.write(pos);                   // tell servo to go to position in variable 'pos'
    delay(15);                            // waits 15ms for the servo to reach the position
  }

  for (pos = percent; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);               // tell servo to go to position in variable 'pos'
    delay(15);                        // waits 15ms for the servo to reach the position
  }
}

void logValues(state state_ptr) {

  Serial.println(counter);
  //store_state(state_ptr);
  values_log_transmit = serialize_state(state_ptr);

  if (counter % 10 == 0) {
    Serial.print(values_log_transmit);
    Serial.write("\r\n");
  } else {
    sd.begin(chipSelect, SPI_HALF_SPEED);

    myFile.open(fileName, O_RDWR | O_CREAT | O_AT_END);
    //if (file) {
      myFile.println(values_log_transmit);
    //} else {
    //  Serial.println("Error opening datalog.txt");
    //}
    myFile.close();

  }
  counter++;

}



String serialize_state(state state_ptr) {
  return String("hi");
}

void store_state(state state_ptr) {
  //digitalWrite(sdPower,HIGH);
  sd.begin(chipSelect, SPI_HALF_SPEED);
  myFile.open(fileName, O_RDWR | O_CREAT | O_AT_END);

  myFile.println("Altitude(m): " + serialize_state(state_ptr));

  //delay(1000);
  myFile.close();

  //digitalWrite(sdPower,LOW);
  //delay(100);
}

int lookUpBrakes(state *state_ptr) {

  unsigned long start = micros();//starts timer
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
  unsigned long endTime = micros(); // ends timer
  unsigned long delta = endTime - start; //gets time elapsed

  //Serial.println(delta); // prints time elapsed
  return percent; // return percent

}
