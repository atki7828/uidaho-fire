#include "MyLED.h"

MyLED::MyLED() {
}

MyLED::MyLED(int n) {
    led_pin = n;
    pinMode(led_pin,OUTPUT);
    Serial.begin(9600);
}

void MyLED::on() {
    digitalWrite(led_pin,HIGH);
    Serial.println("on");
}

void MyLED::off() {
    digitalWrite(led_pin,LOW);
    Serial.println("off");

}
