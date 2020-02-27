// Defines the methods for the Initialize Board class

#include "InitializeBoard.h"

InitializeBoard::InitializeBoard()
{
	SX1509_ADDRESS = 0x3E;
	fiveV_EN = 0; 
	fiveV_ISENSE = 1; // SAMD21, analog pin
	eightV_EN = 1; // enable before BOOST_EN
	BOOST_EN = 3; 
	RS232_BUS = 7; // SAMD21, not SX1509
	IR_BUS = 9; 
	PWR_EN = 13;
	EN_TES_BUS = 11;
	PIXELS = 12; //SAMD21 pin

	setup();
}

// properly enables all pins on the board
void InitializeBoard::setup()
{
	delay(500);
	/* if communicating with Iridium through RS-232 port, 
	*  comment out Serial1.begin(9600);
	**/
	//Serial1.begin(9600);    // Iridium bus
	//Serial.begin(9600);     // TES adapter
	
		// enable all required pins, in correct order:
	sx1509.begin(SX1509_ADDRESS);
	delay(100);
	sx1509.pinMode(fiveV_EN,OUTPUT);
	delay(100);
	sx1509.digitalWrite(fiveV_EN,HIGH);
	delay(200);
	sx1509.pinMode(PWR_EN,OUTPUT);
	delay(100);
	sx1509.digitalWrite(PWR_EN,HIGH);
	delay(200);
	sx1509.pinMode(eightV_EN,OUTPUT);
	delay(100);
	sx1509.digitalWrite(eightV_EN,HIGH);
	delay(200);
	sx1509.pinMode(BOOST_EN,OUTPUT);
	delay(100);
	sx1509.digitalWrite(BOOST_EN,HIGH); 
	delay(200);
		// enable iridium bus, and RS232 bus.
	sx1509.pinMode(IR_BUS,OUTPUT);
	delay(100);
	sx1509.digitalWrite(IR_BUS,HIGH);
	delay(200);
	pinMode(RS232_BUS,OUTPUT);
	delay(100);
	digitalWrite(RS232_BUS,HIGH);
	delay(200);

	pinMode(13,OUTPUT);
	digitalWrite(13,HIGH);
}
