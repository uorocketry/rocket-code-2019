
void setup() {
  Serial.begin(57600);
  delay(1000);
}

void loop() {  
  Serial.write(0x48);   // H
  Serial.write(0x45);   // E
  Serial.write(0x4C);   // L
  Serial.write(0x4C);   // L
  Serial.write(0x4F);   // O   
  Serial.write("\r\n");
//  Serial.write("\r\n");
  delay(1000);
}
