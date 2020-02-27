#ifndef INITIALIZEBOARD_H
#define INITIALIZEBOARD_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SparkFunSX1509.h>

/* code to power on iridium 9523 unit,
 *  and enable all communication buses.
 *  requires sx1509 lib, as our SAMD21 mcu communicates with iridum through sx1509 chip via i2c.
 *  refer to Iridium User Guide for correct order of operations, and pin numbers. 
 */

/*
 * note:  samd21 has Serial1 and Serial.
 * Serial goes to TES bus, and Serial1 goes to Iridium and/or to RS232 (if enabled).
 * these will be used for data i/o (i.e., AT commands) to Iridium.
 *  
 *  To output responses to the Arduino IDE Serial Monitor, call SerialUSB.print(str).
 */

class InitializeBoard
{
	public:
		InitializeBoard();

	protected:
		
	private:
		byte SX1509_ADDRESS;
		SX1509 sx1509;
		int fiveV_EN; 
		int fiveV_ISENSE; // SAMD21, analog pin
		int eightV_EN; // enable before BOOST_EN
		int BOOST_EN; 
		int RS232_BUS; // SAMD21, not SX1509
		int IR_BUS; 
		int PWR_EN;
		int EN_TES_BUS;
		int PIXELS; //SAMD21 pin

		void setup(); // enable pins in correct order

		//const Adafruit_NeoPixel strip(4, PIXELS, NEO_RGB + NEO_KHZ800);
};

#endif 
