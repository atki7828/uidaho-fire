// Team F.I.R.E. - University of Idaho Computer Science (2019-2020)
// File: Iridium.h
// Functionality: Library for handling the Iridium class and its associated
// 				  communication functionality via SBD (Short Burst Data) and
//				  real time dial up data transfer.

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

// Pins we have to enable - Listed in order to enable
#define fiveV_EN 0
#define PWR_EN 13
#define eightV_EN 1  // enable before three959_EN
#define three959_EN 3
#define IR_BUS 9
#define RS232_BUS 7  // SAMD21, not SX1509
#define EN_TES_BUS 11 // We need this to send data from the TES adapter to the microcontroller
#define PIXELS 12  // SAMD21 pin; for LEDS on test board
#define SX1509_ADDRESS 0x3E
#define three959_GOOD 2

enum communicationState { IDLE, WRITING, READING, INITIATING, DIALING, CONNECTED }; // will need to add more as we go

class Iridium
{
    public:
        Iridium(); // COMPLETE FOR SBD
        void init(); // COMPLETE FOR SBD
        void write(String str);
        String TESInput();
        bool ready();
		void loop(); // COMPLETE FOR SBD
		int available(); // COMPLETE FOR SBD
    void WriteSBD(String); // COMPLETE FOR SBD
		void sendSBD(); // COMPLETE FOR SBD
    void InitiateSession();
    void readSBD();
		String readBuffer(); // COMPLETE FOR SBD
		int checkConnection(); // COMPLETE FOR SBD
		void droppedConnectionProtocol(); // COMPLETE FOR SBD - DO LAST
    void ProcessResponse(String); // parses response from AT command

		// NOW WE MAY BE USING RUDICS SO RETHINK DIAL UP METHODS!!
		void initializeDialUp(); // COMPLETE FOR DIAL UP
		void createInternetStack(); // COMPLETE FOR DIAL UP
		void writeDialUpWrapper(); // COMPLETE FOR DIAL UP
		void sendDialUpWrapper(); // COMPLETE FOR DIAL UP

    void setupBoard();
    private:
        static SX1509 sx1509;
        static String CSQ;
	 	//enum communicationStatus {Idle, SBD, Rudics}; // all possible states for the modem
		communicationState commState;
   void SwitchState(communicationState);
   static String statename(communicationState state);
};

#endif // IRIDIUM_H
