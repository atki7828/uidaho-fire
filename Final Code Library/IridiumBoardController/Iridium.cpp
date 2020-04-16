// Team F.I.R.E. - University of Idaho Computer Science (2019-2020)
// File: Iridium.cpp
// Functionality: Implement the SBD and Dial-up Capabilities requested by NASA Ames.

// Libraries
#include "Iridium.h"

// Namespaces
using namespace std;

// Notes:
// Serial1 : Iridium bus
// Serial : TES bus

// Global Variables
SX1509 Iridium::sx1509;
String Iridium::CSQ = "AT+CSQ\r\n";
Iridium::communicationStatus Iridium::commStatus = Idle;

// Public Functions:

// Initialization will happen when iridiumInstantiation.init() is called
Iridium::Iridium() 
{
	// MAYBE RE-WORK FUNCTIONS DOWN THE ROAD TO UTILIZE THIS?
}

// Initializes the carrier board for communication
void Iridium::init() 
{ 
	setupBoard(); // enable the correct pins in the correct order
	//variableInit(); // REMOVE AND JUST ADD INITS AS GLOBALS
	// CONSIDER ADDING OTHER INITS LIKE TCP/IP STACK?
}

// Returns the number of bytes available on Serial1
int Iridium::available() 
{
  	return Serial1.available();
}

// Send output message to SBD buffer
void Iridium::writeSBD(String outgoingMessage)
{
  this->write("AT+SBDWT=");
  this->write(outgoingMessage);
  this->write("\r\n");
}

// Transmit data by initializing an SBD session
void Iridium::sendSBD()
{

}

// Read from iridium serial buffer and return the string
String Iridium::readBuffer()
{
  if(Serial1.available() > 0) {
  	String response = "";
    //SerialUSB.println("Reading");
  	while(Serial1.available() > 0) 
  	{
  		response += (char)Serial1.read();
  		//SerialUSB.println(response);
      delay(100);
  	}
  
  	return response;
  }
  
}

// Return the connection of the Iridium modem to the network
int Iridium::checkConnection()
{

}

// Method to handle a dropped connection
void Iridium::droppedConnectionProtocol()
{

}

// Initializes a dial-up connection
void Iridium::initializeDialUp()
{

}

// Instantiates a separate class for the TCP/IP stack
void Iridium::createInternetStack()
{

}

// Wrapper method to write a message to a dial up queue
void Iridium::writeDialUpWrapper()
{

}

// Wrapper method to send any queued up data via the dial up connection
void Iridium::sendDialUpWrapper()
{

}


// MICHAEL's
// writes directly to Iridium modem.
// to use in context of AT commands,
// do something like....
// String cmd = "AT+SBDWT=";
// iridium9523.write(cmd);
// iridium9523.write("sending this message!");
// iridium9523.write("\r\n");
void Iridium::write(String str) 
{
  	Serial1.print(str);
}

// Private Functions:

// Enables the correct pins in the correct order
void Iridium::setupBoard()
{
	Serial.begin(9600);
	Serial1.begin(9600);
	SerialUSB.begin(9600);
	sx1509.begin(SX1509_ADDRESS);
	//CSQ = "AT+CSQ\r\n"; declared up top
	sx1509.pinMode(fiveV_EN,OUTPUT);
	sx1509.digitalWrite(fiveV_EN,HIGH);
  delay(100);
	SerialUSB.println("5V initialized");
	sx1509.pinMode(PWR_EN,OUTPUT);
	sx1509.digitalWrite(PWR_EN,HIGH);
	SerialUSB.println("pwr initialized");
  delay(100);
	sx1509.pinMode(eightV_EN,OUTPUT);
	sx1509.digitalWrite(eightV_EN,HIGH);
	SerialUSB.println("8V initialized");
  delay(100);
	//sx1509.pinMode(three959_EN,OUTPUT);
	//sx1509.digitalWrite(three959_EN,HIGH);
	//SerialUSB.println("boost initialized");
	sx1509.pinMode(EN_TES_BUS,OUTPUT);
	sx1509.digitalWrite(EN_TES_BUS,HIGH);
	SerialUSB.println("TES bus initialized");
  delay(100);
	sx1509.pinMode(IR_BUS,OUTPUT);
	sx1509.digitalWrite(IR_BUS,HIGH);
	SerialUSB.println("Iridium bus initialized");
  delay(100);
	pinMode(RS232_BUS,OUTPUT);
	digitalWrite(RS232_BUS,HIGH);
	SerialUSB.println("RS232 bus initialized");
	delay(100);
	pinMode(13,OUTPUT);
	digitalWrite(13,HIGH);
	SerialUSB.println("Done!");
}

// Initialize all private variables
void variableInit()
{
	
}


//for some reason requires delay(10).
//otherwise this only returns individual characters, not full string.
// TODO:
// when receiving from TES port, either:
// immediately transmit whatever came in as SBD, or:
// parse input, process whatever command it includes:  transmit, switch mode, check signal?
// or something else.
String Iridium::TESInput() {
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
