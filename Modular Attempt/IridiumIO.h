#ifndef IRIDIUMIO_H
#define IRIDIUMIO_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SparkFunSX1509.h>

class IridiumIO
{
	public:
		IridiumIO();

	protected:

	private:
		int LED_COUNT;
		int LED_PIN;	 
		Adafruit_NeoPixel strip;
		
		void communicationLoop();
		void initLED();
};

#endif 
