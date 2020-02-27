// Controls main communication loop in and out of the chip for Iridium Modem

#include "IridiumIO.h"

IridiumIO::IridiumIO()
{
	LED_COUNT = 4;
	LED_PIN = 12; // SAMD21 pin
	Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);
	initLED();
	communicationLoop();
}

void IridiumIO::communicationLoop()
{
	// listen for incoming data and then transmit as necessary
	while(1)
	{
		break; // remove later!!!!
  }
}

void IridiumIO::initLED()
{
	strip.begin();

	// initialize all pixels to to off (black)
	for(int i = 0; i < LED_COUNT; i++) 
	{
    	strip.setPixelColor(i,0,0,0);
  	}	

	strip.show(); // push all pixels to off state
}
