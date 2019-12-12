  #include <SparkFunSX1509.h>

/* code to power on iridium unit,
 *  and enable all communication buses.
 *  requires sx1509 lib, as SAMD21 mcu communicates with iridum through sx1509 chip via i2c.
 *  refer to Iridium User Guide for correct order of operations, and pin numbers.
 * 
 */

/*
 * note:  samd21 has Serial1 and Serial.
 * one goes to TES bus, one goes to Iridium and to RS232 (if enabled).
 * these will be used for data i/o (i.e., AT commands) to Iridium.
 * refer to Iridium user guide.
 */
const byte SX1509_ADDRESS = 0x3E;
SX1509 sx1509;
// pin 1 corresponds to 8V_EN; enable after PWR_EN, and before 3959_EN (boost; pin 2).
// pins 0 and 13 on the sx1509 are connect to Iridium's 5V_EN and PWR_EN.
// 5V_EN must be enabled before PWR_EN.
// enable by sending high signal through sx1509.digitalWrite().
const int fiveV_EN = 0; 
const int fiveV_ISENSE = 1; // SAMD21 analog
const int eightV_EN = 1;
const int RS232_BUS = 7;  // SAMD21, not SX1509
const int IR_BUS = 9; 
const int PWR_EN = 13;
const int EN_TES_BUS  = 16;

void setup() {
  delay(500);
  Serial1.begin(9600);
  Serial.begin(9600);
  // put your setup code here, to run once:
  sx1509.begin(SX1509_ADDRESS);
  sx1509.pinMode(fiveV_EN,OUTPUT);
  sx1509.digitalWrite(fiveV_EN,HIGH);
  delay(500);
  //pinMode(fiveV_ISENSE,OUTPUT);
  //delay(500);
  sx1509.pinMode(PWR_EN,OUTPUT);
  sx1509.digitalWrite(PWR_EN,HIGH);
  delay(500);
  sx1509.pinMode(eightV_EN,OUTPUT);
  sx1509.digitalWrite(eightV_EN,HIGH);
  delay(500);
  sx1509.pinMode(IR_BUS,OUTPUT);
  sx1509.digitalWrite(IR_BUS,HIGH);
  delay(500);
  pinMode(RS232_BUS,OUTPUT);
  digitalWrite(RS232_BUS,LOW);

  delay(200);
  pinMode(13,OUTPUT);

  delay(500);
}

void loop() {
  Serial1.write("AT\r\n");
  Serial.write("AT\r\n");
  /*
  if(Serial.available() > 0) {
    Serial.println("TES:");
    while(Serial.available() > 0) {
      Serial.print(Serial.read());
    }
  }
  if(Serial1.available() > 0) {
    Serial1.println("RS232:");
    while(Serial1.available() > 0) {
      Serial1.println(Serial1.read());
    }
  }
  */
  
  if(Serial1.available() > 0) {
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    while(Serial1.available() > 0) {
      int c = Serial1.read();
      Serial.print(c);
    }
  }
    delay(2000);

  // put your main code here, to run repeatedly:

}
