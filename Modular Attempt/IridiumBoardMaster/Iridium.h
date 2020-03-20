#ifndef IRIDIUM_H
#define IRIDIUM_H

#include <Arduino.h>
#include <SparkFunSX1509.h>

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


class Iridium {
    public:
        Iridium();
        void init();
        void write(String);
        String read();
        int available();
    private:
        SX1509 sx1509;
        String CSQ;
};

#endif \\ IRIDIUM_H
