#include <EEPROM.h>
int i = 0;
void setup() {
  Serial.begin(9600);


}

void loop() {
  
  if(i == 10){
    i = 0;
  }
  Serial.println(EEPROM.read(i));
  delay(3000);
  i++;

}
