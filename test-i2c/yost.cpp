
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
  _i2c->write(YOST_REGISTER_TEMP);
  _i2c->write(YOST_I2C_COMMAND);
  _i2c->write(YOST_READ_COMMAND);
  _i2c->endTransmission(false); // end transmission

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
