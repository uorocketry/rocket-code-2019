

void setup() {
  Serial.begin(57600);

  Serial.write("\r\n");
  delay(1000);
  
  Serial.write("ATI\r\n");
  delay(2000);
  
  Serial.write("+++");
  delay(3000);

  Serial.write("ATI\r\n");
  delay(2000);

  Serial.write("ATS4=27\r\n");
  delay(1000);
  
  Serial.write("\r\n");
  delay(2000);
  
  Serial.write("ATO\r\n");
  delay(3000);
}



void loop() {
  
  Serial.write(0x48);   // H
  Serial.write(0x45);   // E
  Serial.write(0x4C);   // L
  Serial.write(0x4C);   // L
  Serial.write(0x4F);   // O
  Serial.write("\r\n");
  delay(2000);
}
