// test of writing classes/headers for arduino.
// just the blink program, but with led as a class

#include "MyLED.h"

int led_pin = 13;
MyLED led;
int del = 500;

void setup() {
  led = MyLED(led_pin);
}

void loop() {
  delay(del);
  led.on();
  delay(del);
  led.off();
}
