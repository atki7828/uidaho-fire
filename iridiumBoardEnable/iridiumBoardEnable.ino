#include <Adafruit_NeoPixel.h>
#include <SparkFunSX1509.h>

/* code to power on iridium 9523 unit,
 *  and enable all communication buses.
 *  requires sx1509 lib, as our SAMD21 mcu communicates with iridum through sx1509 chip via i2c.
 *  refer to Iridium User Guide for correct order of operations, and pin numbers.
 * 
 */

/*
 * note:  samd21 has Serial1 and Serial.
 * Serial goes to TES bus, and Serial1 goes to Iridium and/or to RS232 (if enabled).
 * these will be used for data i/o (i.e., AT commands) to Iridium.
 *  
 *  To output responses to the Arduino IDE Serial Monitor, call SerialUSB.print(str).
 *
 */
 
 //enable in this order:
#define fiveV_EN 0
#define PWR_EN 13
#define eightV_EN 1  // enable before three959_EN
#define three959_EN 3 
#define IR_BUS 9
#define RS232_BUS 7  // SAMD21, not SX1509
#define EN_TES_BUS 11 // i think we'll need this to send data from the TES adapter to the microcontroller.
#define PIXELS 12  //SAMD21 pin
#define SX1509_ADDRESS 0x3E
#define three959_GOOD 2
SX1509 sx1509;
// pin 1 corresponds to 8V_EN; enable after PWR_EN, and before 3959_EN (boost; pin 3).
// pins 0 and 13 on the sx1509 are connect to Iridium's 5V_EN and PWR_EN.
// 5V_EN must be enabled before PWR_EN.
// enable by sending high signal through sx1509.digitalWrite().



Adafruit_NeoPixel strip(4, PIXELS, NEO_RGB + NEO_KHZ800);

void setup() {
  SerialUSB.begin(9600);   // native USB port (arduino ide monitor)
  Serial1.begin(9600);    // Iridium bus
  //Serial.begin(9600);     // TES adapter
  delay(500);
  InitLED();
  
  /* if communicating with Iridium through RS-232 port, 
   *  comment out Serial1.begin(9600);
   **/
  
  // enable all required pins, in correct order:
  sx1509.begin(SX1509_ADDRESS);
  delay(100);
  sx1509.pinMode(fiveV_EN,OUTPUT);
  delay(100);
  sx1509.digitalWrite(fiveV_EN,HIGH);
  delay(200);
  SerialUSB.println("5V enabled");
  sx1509.pinMode(PWR_EN,OUTPUT);
  delay(100);
  sx1509.digitalWrite(PWR_EN,HIGH);
  delay(200);
  SerialUSB.println("power enabled");
  sx1509.pinMode(eightV_EN,OUTPUT);
  delay(100);
  sx1509.digitalWrite(eightV_EN,HIGH);
  delay(200);
  SerialUSB.println("8V enabled");
  sx1509.pinMode(three959_EN,OUTPUT);
  delay(100);
  sx1509.digitalWrite(three959_EN,HIGH);
  delay(200);
  SerialUSB.println("boost enabled");
    // enable iridium bus, and RS232 bus.
      sx1509.pinMode(EN_TES_BUS,OUTPUT);
  delay(100);
  sx1509.digitalWrite(EN_TES_BUS,HIGH);
  delay(200);
  SerialUSB.println("TES bus enabled");
  sx1509.pinMode(IR_BUS,OUTPUT);
  delay(100);
  sx1509.digitalWrite(IR_BUS,HIGH);
  delay(200);
  SerialUSB.println("Iridium bus enabled");
  pinMode(RS232_BUS,OUTPUT);
  delay(100);
  digitalWrite(RS232_BUS,HIGH);
  delay(200);
  SerialUSB.println("RS232 bus enabled");

  pinMode(13,OUTPUT);
  digitalWrite(13,HIGH);
  SerialUSB.println("done.");
    

}

void loop() {
  /* test commands:
   *  send:         receive:
   *  AT\r\n        OK\r\n
   *  AT+CSQ\r\n    CSQ: [0-5]\r\n
   *  
   */

// comment out everything to write through the RS232 port.
// 
  //Serial1.write("AT\r\n");

   /**/
  // if iridium is responding, print to serial monitor:
  if(Serial1.available() > 0) {
    blink();
    while(Serial1.available() > 0) {
      SerialUSB.print((char)Serial1.read());
    }
    SerialUSB.print('\n');
    delay(10);
  }

  // from arduino serial monitor (Tools > Serial Monitor),
  //we can send messages to the iridium.
  if(SerialUSB.available() > 0) {
    while(SerialUSB.available() > 0) {
      Serial1.write(SerialUSB.read());
    }
    Serial1.write("\r\n");
  }

  /* instead of reading from Arduino IDE Serial Monitor,
  //  we can read from TES bus adapter.
    // TES adapter can be plugged into USB, so we can write to it
    // from a laptop's USB connection,
    // or rx/tx pins -> raspberry pi.
  if(Serial.available() > 0) {
    while(Serial.available() > 0) {
      Serial1.write(Serial.read());
    }
    Serial1.write("\r\n");
  }
  delay(200);

  /**/

}


void blink() {
  digitalWrite(13,LOW);
  delay(100);
  digitalWrite(13,HIGH);
}

void InitLED() {
  strip.begin();
  /*
   * to control LEDs, use strip.setPixelColor(LEDnum,R,G,B);
   * LEDnum = which LED to light (0 - 3)
   * RGB are red/green/blue values from 0 - 255.
   * note:  255 is reeaally bright.
   */
   /*
   strip.setPixelColor(0,32,0,32);
   strip.setPixelColor(1,32,32,0);
   strip.setPixelColor(2,0,32,0);
   strip.setPixelColor(3,0,32,32);
   */
   
  for(int i = 0; i < 4; i++) {
    strip.setPixelColor(i,0,0,0);
  }
  
  strip.show();
}
