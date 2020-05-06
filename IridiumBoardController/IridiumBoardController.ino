// Team F.I.R.E. - University of Idaho Computer Science (2019 - 2020)
// CS Members: Michael Atkinson, Brandon Foss, Lennin Rodriguez

// Code: Iridium 9523 Communication Library
// Goal: Develop a software package that utilizes the carrier board
//		 re-designed by the Electrical Engineers to communicate between
//		 an Iridium 9523 and a ground based server via SBD (Short Burst Data)
//       and dial up as a real time data transfer method.

// IridiumBoardController.ino:
//       -The main controller of our overall software package.
//       -Initiates all relevant systems and then updates each of them through the 
//		  update method.

/*
* In order to handle commands for data transmission and reception,
* we're implementing a simple command set, such that the microcontroller will receive 
* a certain command and take action based on it.
* These are the commands we're using so far:
*  Command:        Action:
*  tx:[data]       transmit SBD
*  rx              check for incoming SBD
*  dial            establish dialup connection
*  csq             check signal quality
*/

// Libraries:
#include "Iridium.h"
#include <SparkFunSX1509.h>

// Global Variables:
Iridium iridium9523; // Instance of our Iridium class
String TESInputBuffer;

// Main Functions

// Initialize the carrier board with the appropriate pins
void setup()
{
    SerialUSB.begin(9600);
    SerialUSB.println("initializing....");
    delay(2000); // Delay by 1 second to give IDE's serial monitor time to load
    iridium9523.setupBoard(); // Initialize the carrier board and communication
}


void loop()
{
    if(SerialUSB.available() > 0) 
    {
        String input = IDEInput();
        SerialUSB.print("you said: ");
        SerialUSB.println(input);
        iridium9523.write(input + "\r\n");
    }

    // append input from TES to TESInputBuffer, with \r\n as delimiter
    if(TESSer.available() > 0) 
    {
        TESInputBuffer += GetTESInput() + "\r\n";
        SerialUSB.print("TES said: ");
        SerialUSB.println(TESInputBuffer);
    }

    // if there's ready input from TES,
    // and if the iridium's not busy,
    // do the thing.
    if(TESInputBuffer.length() > 0 && iridium9523.ready()) 
    {
        String input = TESInputBuffer.substring(0,TESInputBuffer.indexOf("\r\n")+1);
        HandleTESInput(input);
        // pop the processed message out of the buffer:
        TESInputBuffer = TESInputBuffer.substring(TESInputBuffer.indexOf("\r\n")+2,TESInputBuffer.length());
    }

    if(IridiumSer.available() > 0) {
        String response = iridium9523.readBuffer();
        SerialUSB.print("iridium said: ");
        SerialUSB.println(response);
        iridium9523.processResponse(response);
    }
}

void HandleTESInput(String input) 
{
    if(input.indexOf(TX_COMM) > -1) 
    {
        iridium9523.writeSBD(input.substring(3,input.length()));
    }
    else if(input.indexOf(RX_COMM) > -1) 
    {

    }
    else if(input.indexOf(DIAL_COMM) > -1) 
    {
        iridium9523.initializeDialUp();
    }
}

// read from arduino IDE serial monitor
String IDEInput() 
{
    if(SerialUSB.available() > 0) 
    {
        String r = "";
        while(SerialUSB.available() > 0) 
        {
            r += (char)SerialUSB.read();
        }
        return r;
    }
}


//for some reason requires delay(10). (10ms)
//otherwise this only returns individual characters, not full string.
// TODO:
// when receiving from TES port, either:
// immediately transmit whatever came in as SBD, or:
// parse input, process whatever command it includes:  transmit, switch mode, check signal?
// or something else.
String GetTESInput() 
{
    if(TESSer.available() > 0)
    {
        SerialUSB.println("reading tes");
        String r = "";
        while(TESSer.available() > 0) 
        {
            r += (char)TESSer.read();
            delay(10);
        }
        return r;
    }  
}
