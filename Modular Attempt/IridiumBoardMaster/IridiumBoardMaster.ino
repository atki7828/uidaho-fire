#include "Iridium.h"

// constructor is called here,
Iridium iridium;
// and some of the setup stuff can't be done outside of setup function.
// I think that's why our code killed the chip!

void setup() {
  // IDE's serial monitor takes a minute to load....
  // so to get init output, delay 1 second.
  delay(1000);
  iridium.init();
  
}

void loop() {
    // if iridium is responding, print to serial monitor:
  if(Serial1.available() > 0) {
    blink();
    String r;
    while(Serial1.available() > 0) {
      r += (char)Serial1.read();
      //SerialUSB.print((char)Serial1.read());
    }
    SerialUSB.println(r);
    delay(10);
  }

  // from arduino serial monitor (Tools > Serial Monitor),
  //we can send messages to the iridium.
  if(SerialUSB.available() > 0) {
    String r;
    while(SerialUSB.available() > 0) {
      Serial1.write(SerialUSB.read());
    }
    Serial1.write("\r\n");
  }
}


void blink() {
  digitalWrite(13,LOW);
  delay(200);
  digitalWrite(13,HIGH);
}
