#ifndef MYLED_H
#define MYLED_H
// need to include Arduino.h to use 
// built in functions like pinMode() and DigitalWrite().
#include <Arduino.h>

class MyLED {
    public:
        MyLED();
        MyLED(int);
        void on();
        void off();

    private:
        int led_pin;
        int delay;

};

#endif //MYLED_H
