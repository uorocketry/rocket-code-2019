#include <EEPROM.h>

int addr = 0;
int val1 = 10;
int val2 = 20;
int val3 = 30; 
int val4 = 40;
int val5 = 50;
int val6 = 80;
int val7 = 90;

void setup()
{
  Serial.begin(9600);
  Serial.println(EEPROM.length());
  for(int i=0; i<4; i++){
    for( int j=0;  j<13; j++){
      EEPROM.write(addr, val1);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0;  j<5;  j++){
      EEPROM.write(addr, val2);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0;  j<2;  j++){
      EEPROM.write(addr, val3);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0;  j<2;  j++){
      EEPROM.write(addr, val4);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<4; j++){
      EEPROM.write(addr, val5);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<1; j++){
      EEPROM.write(addr, val6);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<5; j++){
      EEPROM.write(addr, val7);
      addr = addr + 1;
      delay(100);
    }
  }
  val1 = val1--;
  val2 = val2--;
  val3 = val3--;
  val4 = val4--;
  val5 = val5--;
  val6 = val6--;
  val7 = val7--;
  
  for(int i=0; i<3; i++){
    for(int j=0; j<13;j++){
      EEPROM.write(addr, val1);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<5; j++){
      EEPROM.write(addr, val2);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<2; j++){
      EEPROM.write(addr, val3);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<2; j++){
      EEPROM.write(addr, val4);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<4; j++){
      EEPROM.write(addr, val5);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<1; j++){
      EEPROM.write(addr, val6);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<5; j++){
      EEPROM.write(addr, val7);
      addr = addr + 1;
      delay(100);
    }
  }

  for(int i=0; i<9; i++){
    val1 = val1--;
    val2 = val2--;
    val3 = val3--;
    val4 = val4--;
    val5 = val5--;
    val6 = val6--;
    val7 = val7--;
    for(int j=0; j<13;j++){
      EEPROM.write(addr, val1);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<5; j++){
      EEPROM.write(addr, val2);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<2; j++){
      EEPROM.write(addr, val3);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<2; j++){
      EEPROM.write(addr, val4);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<4; j++){
      EEPROM.write(addr, val5);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<1; j++){
      EEPROM.write(addr, val6);
      addr = addr + 1;
      delay(100);
    }
    for(int j=0; j<5; j++){
      EEPROM.write(addr, val7);
      addr = addr + 1;
      delay(100);
    }
  }
  EEPROM.write(0, 0);
  EEPROM.write(1023, 0);


}

void loop()
{
  
}
