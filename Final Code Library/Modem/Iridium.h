// Team F.I.R.E. - University of Idaho Computer Science (2019-2020)
// File: Iridium.h
// Functionality: Library for handling the Iridium class and its associated 
// 				  communication functionality via SBD (Short Burst Data) and 
//				  real time dial up data transfer.

#ifndef IRIDIUM_H
#define IRIDIUM_H

// Libaries
#include <Arduino.h>
#include <SparkFunSX1509.h>
#include <string>

// Pins we have to enable - Listed in order to enable
#define fiveV_EN 0
#define PWR_EN 13
#define eightV_EN 1  // enable before three959_EN
#define three959_EN 3 
#define IR_BUS 9
#define RS232_BUS 7  // SAMD21, not SX1509
#define EN_TES_BUS 11 // We need this to send data from the TES adapter to the microcontroller
#define PIXELS 12  // SAMD21 pin
#define SX1509_ADDRESS 0x3E
#define three959_GOOD 2


class Iridium 
{
    public:
        Iridium(); // COMPLETE FOR SBD 
        void init(); // COMPLETE FOR SBD 
		void loop(); // COMPLETE FOR SBD
		int available(); // COMPLETE FOR SBD
        void writeSBD(String); // COMPLETE FOR SBD 
		void sendSBD(); // COMPLETE FOR SBD 
		string readBuffer(); // COMPLETE FOR SBD 
		int checkConnection(); // COMPLETE FOR SBD 
		void droppedConnectionProtocol(); // COMPLETE FOR SBD - DO LAST
		
		// NOW WE ARE USING RUDICS SO RETHINK DIAL UP METHODS!!
		void initializeDialUp(); // COMPLETE FOR DIAL UP
		void createInternetStack(); // COMPLETE FOR DIAL UP
		void writeDialUpWrapper(); // COMPLETE FOR DIAL UP
		void sendDialUpWrapper(); // COMPLETE FOR DIAL UP

		void changeCommunicationMode(); // COMPLETE TO SWITCH BETWEEN COMM MODES
    private:
        static SX1509 sx1509;
        static string CSQ;
	 	static enum communicationStatus {Idle, SBD, Rudics}; // all possible states for the modem
		static communicationStatus commStatus;
};

#endif // IRIDIUM_H