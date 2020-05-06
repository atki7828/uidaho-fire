// Team F.I.R.E. - University of Idaho Computer Science (2019-2020)
// File: Iridium.h
// Functionality: Library for handling the Iridium class and its associated communication
//                functionality via SBD (Short Burst Data) and real time dial up data transfer.			  

#ifndef IRIDIUM_H
#define IRIDIUM_H

// Serial ports
#define IridiumSer    Serial1
#define TESSer        Serial

// iridium commands
#define TX_COMM       "tx:"
#define RX_COMM       "rx"
#define DIAL_COMM     "dial"
#define CSQ_COMM      "csq"
#define GatewayNumber "008816000025"

// Libaries
#include <Arduino.h>
#include <SparkFunSX1509.h>
#include <string.h>

// Pins we have to enable - Listed in the order they are enabled
#define fiveV_EN        0
#define PWR_EN          13
#define eightV_EN       1  // enable before three959_EN
#define three959_EN     3
#define IR_BUS          9
#define RS232_BUS       7  // SAMD21, not SX1509
#define EN_TES_BUS      11 // Needed to send data from TES adapter -> microcontroller
#define PIXELS          12  // SAMD21 pin; for LEDS on test board
#define SX1509_ADDRESS  0x3E
#define three959_GOOD   2

// Stores the state of the Iridium - Capable of switching depending on desired comm method
enum communicationState { IDLE, WRITING, READING, INITIATING, DIALING, CONNECTED }; 

// Main class for controlling the Iridium modem through the onboard microprocessor
class Iridium
{
public:
    Iridium(); 
    void setupBoard();
    void WriteSBD(String str); 
    void write(String str);
    void InitiateSession();
    String readBuffer();
    int available();
    bool ready(); 
    void ProcessResponse(String str); 
    void initializeDialUp(); 
private:
    static SX1509 sx1509;
    static String CSQ;
    static int baud;
    communicationState commState;
    void SwitchState(communicationState state);
    static String statename(communicationState state);
};

#endif // IRIDIUM_H
