//Component: Yost 3-Space™ LX Embedded part number TSS-LX

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>

#include "Yost.h"

/**************************************************************************/
/*!
    @brief  Instantiates a new Yost class
*/
/**************************************************************************/
Yost::Yost() {

}


/**************************************************************************/
/*!
    @brief  Setups the HW (reads coefficients values, etc.)
    @param twoWire Optional TwoWire I2C object
    @return true on successful startup, false otherwise
*/
/**************************************************************************/
boolean Yost::begin(TwoWire *twoWire) {
  _i2c = twoWire;
  _i2c->begin();


 //TODO: Later, we will need to do initialization stuff here
  /*
  uint8_t whoami = read8(MPL3115A2_WHOAMI);
  if (whoami != 0xC4) {
    return false;
  }

  write8(MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST);
  delay(10);

  while(read8(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_RST) delay(10);

  _ctrl_reg1.reg = MPL3115A2_CTRL_REG1_OS128 | MPL3115A2_CTRL_REG1_ALT;

  write8(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);

  write8(MPL3115A2_PT_DATA_CFG,
	 MPL3115A2_PT_DATA_CFG_TDEFE |
	 MPL3115A2_PT_DATA_CFG_PDEFE |
	 MPL3115A2_PT_DATA_CFG_DREM);
  */
  return true;
}


/**************************************************************************/
/*!
    @brief  Gets the floating-point temperature in Centigrade
    @return temperature reading in Centigrade as a floating-point value
*/
/**************************************************************************/

float Yost::getTemperature() {
  int16_t t;


//TODO: Find out what status register is
  /*
  uint8_t sta = 0;
  while (! (sta)) {
    _i2c->beginTransmission(YOST_ADDRESS); // start transm ission to device
    _i2c->write(YOST_STATUS_COMMAND);
    _i2c->endTransmission(false); // end transmission

    sta = read8(MPL3115A2_REGISTER_STATUS);
    delay(10);
  }
  */
  _i2c->beginTransmission(YOST_ADDRESS); // start transmission to device
  _i2c->write(YOST_I2C_COMMAND);
  _i2c->write(YOST_WRITE_COMMAND);
  _i2c->write(YOST_REGISTER_TEMP); //write read temp command to imu
  _i2c->write(YOST_I2C_COMMAND);
  _i2c->write(YOST_READ_COMMAND);
  _i2c->endTransmission(false); // end transmission

  // unsigned char same as uint8_t (size of char is 1 byte)
  // Cast (type)variable
  // The cast operator translates one variable type into another and forces calculations to be performed in the cast type.
  _i2c->requestFrom((uint8_t)YOST_ADDRESS, (uint8_t)4);// send data n-bytes read
  for (int i = 0; i<4; i++){
    temp.tempArray[3-i] = _i2c->read(); // receive DATA
  }

//TODO: check out what this does
  if (t & 0x800) {
    t |= 0xF000;
  }

  return temp.tempFloat;
}


float *Yost::read_orientation_euler() {

  static float angle_euler[3];
  byte buffer[12];

  // 1 btye = 8 bits
  int16_t t;
  _i2c->beginTransmission(YOST_ADDRESS); // transmit to register (0x77)
  _i2c->write(YOST_I2C_COMMAND); // prepares imu to write (0xEE)
  _i2c->write(YOST_WRITE_COMMAND); // send write command to imu (0x42)
  _i2c->write(YOST_REGISTER_EULER); // write euler command to imu (0x01)
  _i2c->write(YOST_I2C_COMMAND); // (0xEE)
  _i2c->write(YOST_READ_COMMAND);
  _i2c->endTransmission(false); // stops transmitting then transmits the bytes that were queued by write()

  //Wire.requestFrom(address, quantity)
  _i2c->requestFrom((uint8_t)YOST_ADDRESS, (uint8_t)12); // dump 12 bytes of data onto buffer
    for (int i = 0; i<12; i++){
      buffer[11-i] = _i2c->read(); // receive DATA
    }

  for(int i = 0; i < 4; i++){
    euler.eulerArray[3-i] = buffer[3-i]; // put 1st 4 byte segment onto eulerArray
  }

  angle_euler[0] = euler.eulerFloat; // turn byte array to float (union)

  for(int i = 0; i < 4; i++){
    euler.eulerArray[3-i] = buffer[7-i]; // put 2nd 4 byte segment onto eulerArray
  }

  angle_euler[1] = euler.eulerFloat; // turn byte array to float (union)

  for(int i = 0; i < 4; i++){
    euler.eulerArray[3-i] = buffer[11-i]; // put 3rd 4 byte segment onto eulerArray
  }

  angle_euler[2] = euler.eulerFloat; // turn byte array to float (union)


  //TODO: check out what this does
  if (t & 0x800) {
    t |= 0xF000;
  }

  return angle_euler;
}


float *Yost::read_accel_filtered() {
  // 1 btye = 8 bits

  byte buffer[12];

  static float xyz_accel[3];

  int16_t t;
  _i2c->beginTransmission(YOST_ADDRESS); // transmit to register (0x77)
  _i2c->write(YOST_I2C_COMMAND); // prepares imu to write (0xEE)
  _i2c->write(YOST_WRITE_COMMAND); // send write command to imu (0x42)
  _i2c->write(YOST_REGISTER_ACCEL); // write Read accelerometer command to imu (0x27)
  _i2c->write(YOST_I2C_COMMAND); // (0xEE)
  _i2c->write(YOST_READ_COMMAND);
  _i2c->endTransmission(false); // stops transmitting then transmits the bytes that were queued by write()

  _i2c->requestFrom((uint8_t)YOST_ADDRESS, (uint8_t)12); // dump 12 bytes of data onto buffer
    for (int i = 0; i<12; i++){
      buffer[11-i] = _i2c->read(); // receive DATA
    }

  for(int i = 0; i < 4; i++){
    accel.accelArray[3-i] = buffer[3-i]; // put 1st 4 byte segment onto accelArray
  }

  xyz_accel[0] = accel.accelFloat; // turn byte array to float (union)

  for(int i = 0; i < 4; i++){
    accel.accelArray[3-i] = buffer[7-i]; // put 1st 4 byte segment onto accelArray
  }

  xyz_accel[1] = accel.accelFloat; // turn byte array to float (union)

  for(int i = 0; i < 4; i++){
    accel.accelArray[3-i] = buffer[11-i]; // put 1st 4 byte segment onto accelArray
  }

  xyz_accel[2] = accel.accelFloat; // turn byte array to float (union)

  //TODO: check out what this does
  if (t & 0x800) {
    t |= 0xF000;
  }

  return xyz_accel;    // Return memory address of array
  }


void Yost::set_axis_directions_with_tare() {
  int16_t t;

  _i2c->beginTransmission(YOST_ADDRESS); // transmit to register (0x77)
  _i2c->write(YOST_I2C_COMMAND); // prepares imu to write (0xEE)
  _i2c->write(YOST_WRITE_COMMAND); // send write command to imu (0x42)
  _i2c->write(YOST_REGISTER_AXIS_W_TARE); // write set axis with tare command to imu (0x60)
  _i2c->write(YOST_I2C_COMMAND); // (0xEE)
  _i2c->write(YOST_READ_COMMAND);
  _i2c->endTransmission(false); // stops transmitting then transmits the bytes that were queued by write()


  //TODO: check out what this does
  if (t & 0x800) {
    t |= 0xF000;
 }
}


 void Yost::software_reset() {
  int16_t t;

  _i2c->beginTransmission(YOST_ADDRESS); // transmit to register (0x77)
  _i2c->write(YOST_I2C_COMMAND); // prepares imu to write (0xEE)
  _i2c->write(YOST_WRITE_COMMAND); // send write command to imu (0x42)
  _i2c->write(YOST_REGISTER_SW_RESET); // write software reset command to imu (0xE2)
  _i2c->write(YOST_I2C_COMMAND); // (0xEE)
  _i2c->write(YOST_READ_COMMAND);
  _i2c->endTransmission(false); // stops transmitting then transmits the bytes that were queued by write()


  //TODO: check out what this does
  if (t & 0x800) {
    t |= 0xF000;
 }

}
