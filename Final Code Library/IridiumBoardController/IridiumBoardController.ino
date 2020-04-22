// Team F.I.R.E. - University of Idaho Computer Science
// CS Members: Michael Atkinson, Brandon Foss, Lennin Rodriguez
// CS/EE Member: Koffi Anderson
// EE Members: Collin Heist, Shelby Linafelter
// ME/MSE Members: MacKenzie Sexton, Sesily Stewart

// Code: Iridium 9523 Communication Library
// Goal: Develop a software package that utilizes the carrier board
//		 re-designed by the Electrical Engineers to communicate between
//		 an Iridium 9523 and a ground based server via SBD (Short Burst Data)
//       and dial up as a real time data transfer method.

// IridiumBoardController.ino:
//     -The main controller of our overall software package.
//     -Initiates all relevant systems and then updates each of them through the 
//		update method.

/*
 * In order to handle commands for data transmission and reception,
 * we're implementing a simple command set, such that the microcontroller will receive a certain command and take action based on that.
 * these are the commands we're using so far:
 *  command:      action:
 *  tx:[data]       transmit SBD
 *  rx              check for incoming SBD
 *  
 */
// Libraries
#include "Iridium.h"
#include <SparkFunSX1509.h>

// Global Variables
Iridium iridium9523; // Instance of our Iridium class

// Main Functions

// Initialize the carrier board with the appropriate pins
void setup()
{
  SerialUSB.begin(9600);
  SerialUSB.println("initializing....");
	delay(2000); // Delay by 1 second to give IDE's serial monitor time to load
	iridium9523.init(); // Initialize the carrier board and communication
}

//
void loop()
{
	//iridium9523.loop(); // Perform iridium9523 loop functions.
  if(SerialUSB.available() > 0) {
    String input = IDEInput();
    SerialUSB.print("you said: ");
    SerialUSB.println(input);
    iridium9523.write(input + "\r\n");
  }
  if(TESSer.available() > 0) {
    String input = TESInput();
    SerialUSB.print("TES said: ");
    SerialUSB.println(input);
    SerialUSB.println(input.indexOf("tx:"));
    if(input.indexOf("tx:") > -1) {
      SerialUSB.println(input.substring(3,input.length()));
      iridium9523.WriteSBD(input.substring(3,input.length()));
    }
  }
  if(IridiumSer.available() > 0) {
    String response = iridium9523.readBuffer();
    SerialUSB.print("iridium said: ");
    SerialUSB.println(response);
    iridium9523.ProcessResponse(response);
  }

}

// read from arduino IDE serial monitor
String IDEInput() {
  if(SerialUSB.available() > 0) {
    String r = "";
    while(SerialUSB.available() > 0) {
      r += (char)SerialUSB.read();
    }
    return r;
  }
}


//for some reason requires delay(10). (that's 10ms)
//otherwise this only returns individual characters, not full string.
// TODO:
// when receiving from TES port, either:
// immediately transmit whatever came in as SBD, or:
// parse input, process whatever command it includes:  transmit, switch mode, check signal?
// or something else.
String TESInput() {
  if(Serial.available() > 0) {
    SerialUSB.println("reading tes");
    String r = "";
    while(Serial.available() > 0) {
      r += (char)Serial.read();
      delay(10);
    }
    return r;
  }  
}

// Blinks pin 13 on our board for current testing - REMOVE IN FINAL RELEASE
void blink()
{
	digitalWrite(13,LOW);
  	delay(200);
  	digitalWrite(13,HIGH);
}
