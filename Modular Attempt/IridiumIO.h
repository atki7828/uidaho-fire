#ifndef IRIDIUMIO_H
#define IRIDIUMIO_H

#include <Arduino.h>
#include "InitializeBoard.h"

class IridiumIO
{
	public:
		IridiumIO();

	protected:

	private:
		int LED_COUNT;
		int LED_PIN;	 
		
		void communicationLoop();
		void initLED();
};

#endif 
