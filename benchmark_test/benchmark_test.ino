#include <math.h>
#include <EEPROM.h>
void setup() {
  Serial.begin(9600);

}

void loop() {
  
  

  Serial.println(function2(20, 80)==12); // test case to see if lookup table works, more test cases below
  
  /**
  Serial.println(function2(20, 110)==15);
  delay(1000);
  Serial.println(function2(20, 210)==0);
  delay(1000);
  Serial.println(function2(20, 310)==0);
  delay(1000);
  Serial.println(function2(20, 410)==10);
  delay(1000);
  Serial.println(function2(20, 510)==10);
  delay(1000);
  Serial.println(function2(20, 610)==15);
  delay(1000);
  Serial.println(function2(20, 710)==0);
  delay(1000);
  Serial.println(function2(20, 810)==12);
  delay(1000);
  Serial.println(function2(20, 910)==12);
  delay(1000);
  Serial.println(function2(20, 1010)==8);
  delay(1000);
  Serial.println(function2(20, 1110)==1);
  delay(1000);
  Serial.println(function2(20, 1210)==7);
  delay(1000);
  Serial.println(function2(20, 1310)==1);
  delay(1000);
  Serial.println(function2(20, 1410)==8);
  delay(1000);
  Serial.println(function2(20, 1510)==8);
  delay(1000);
  Serial.println(function2(20, 1610)==9);
  delay(1000);
  Serial.println(function2(20, 1710)==0);
  delay(1000);
  Serial.println(function2(20, 1810)==3);
  delay(1000);
  Serial.println(function2(20, 1910)==3);
  delay(1000);
  **/
  
  
  delay(1000);
}
//computational method
double function1(double velocity, double brake_deploy){;
  double drag = 0.00912129504354*(brake_deploy + 0.484797983795)*square(velocity);
  return drag;
}
//gets airbrake from lookup table
int function2(int velocity, int altitude){
  unsigned long start = micros();//starts timer
  unsigned int first_four_bits; // first four bits of the address
  //switch case sets first four bits of address based off the velocity
  switch(velocity) {
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
  switch(altitude){
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
  int x = first_four_bits*16+(last_five_bits/2); // concatanates both adress components arithmetically
  int b = EEPROM.read(x); // gets EEPROM value at address x(8 bit int)
  int percent; // airbreak percent 
  if(last_five_bits%2==0){ // if last 9th bit is 0 it uses the first 4 bits of EEPROM value at x
    percent = b/16; // divides by 16 getting rid of the last 4 bits, only leaving the first 4 bits
  }
  else {
    percent = b-(b/16)*16; // gets last 4 bits 
  }
  unsigned long endTime = micros(); // ends timer
  unsigned long delta = endTime - start; //gets time elapsed
  
  Serial.println(delta); // prints time elapsed
  return percent; // return percent
  
}
