#include "Iridium.h"

// Serial1 : Iridium bus
// Serial : TES bus

Iridium::Iridium() {

}


void Iridium::init() { 
  Serial.begin(9600);
  Serial1.begin(9600);
  SerialUSB.begin(9600);
  sx1509.begin(SX1509_ADDRESS);
  SerialUSB.println("constructor constructed!");
  CSQ = "AT+CSQ\r\n";
  sx1509.pinMode(fiveV_EN,OUTPUT);
  sx1509.digitalWrite(fiveV_EN,HIGH);
  SerialUSB.println("5V initialized");
  sx1509.pinMode(PWR_EN,OUTPUT);
  sx1509.digitalWrite(PWR_EN,HIGH);
  SerialUSB.println("pwr initialized");
  sx1509.pinMode(eightV_EN,OUTPUT);
  sx1509.digitalWrite(eightV_EN,HIGH);
  SerialUSB.println("8V initialized");
  //sx1509.pinMode(three959_EN,OUTPUT);
  //sx1509.digitalWrite(three959_EN,HIGH);
  //SerialUSB.println("boost initialized");
  sx1509.pinMode(EN_TES_BUS,OUTPUT);
  sx1509.digitalWrite(EN_TES_BUS,HIGH);
  SerialUSB.println("TES bus initialized");
  sx1509.pinMode(IR_BUS,OUTPUT);
  sx1509.digitalWrite(IR_BUS,HIGH);
  SerialUSB.println("Iridium bus initialized");
  pinMode(RS232_BUS,OUTPUT);
  digitalWrite(RS232_BUS,HIGH);
  SerialUSB.println("RS232 bus initialized");
  
  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  SerialUSB.println("done.");
  /**/
}

void Iridium::write(String str) {
  Serial1.print(str);
}

String Iridium::read() {
  String response = "";
  while(Serial1.available() > 0) {
    SerialUSB.println("reading");
    response += Serial1.read();
    SerialUSB.println(response);
  }
  return response;
}

int Iridium::available() {
  return Serial1.available();
}
