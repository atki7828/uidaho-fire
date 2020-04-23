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
  	return IridiumSer.available();
}

// Send output message to SBD buffer
void Iridium::WriteSBD(String outgoingMessage)
{
  SerialUSB.print("writing sbd:");
  SerialUSB.println(outgoingMessage);
  this->write("AT+SBDWT=");
  this->write(outgoingMessage);
  this->write("\r\n");
  this->commState = WRITING;
}

// initiates a session with the satellite network
void Iridium::InitiateSession() {
  SerialUSB.println("initiating session");
  this->write("AT+SBDI\r\n");
}

// Transmit data by initializing an SBD session
void Iridium::sendSBD()
{

}

// Read from iridium serial buffer and return the string
String Iridium::readBuffer()
{
  if(IridiumSer.available() > 0) {
  	String response = "";
    //SerialUSB.println("Reading");
  	while(IridiumSer.available() > 0) 
  	{
  		response += (char)IridiumSer.read();
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
  	IridiumSer.print(str);
}

// Private Functions:

// Enables the correct pins in the correct order
void Iridium::setupBoard()
{
	TESSer.begin(9600);
	IridiumSer.begin(9600);
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
	sx1509.pinMode(three959_EN,OUTPUT);
	sx1509.digitalWrite(three959_EN,HIGH);
	SerialUSB.println("boost initialized");
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
  this->commState = IDLE;
}

// take an action based on iridium's response to our commands.
void Iridium::ProcessResponse(String response) {
  if(response.indexOf("OK") > 0) {
    switch(this->commState) {
      case WRITING:
        this->InitiateSession();
        this->commState = CONNECTING;
        break;
      default:
        this->commState = IDLE;
        break;
    }
    return;
  }
  else if(response.indexOf("SBDRT:") > 0) {
    // response was:  "+SBDRT:\r\n(incoming message)\r\n".
    // we need to store "(incoming message)" in it's own variable, and...do something with it.  maybe just print to SerialUSB for now.
    this->commState = IDLE;
    return;
  }
  else if(response.indexOf("SBDI:") > 0) {
    /*
     * +SBDI:<MO status>,<MOMSN>,<MT status>,<MTMSN>,<MT length>,<MT queued>
        where:
        <MO status>:
          MO session status provides an indication of the disposition of the mobile originated transaction.
          The field can take on the following values:
          0: No SBD message to send from the ISU.
          1: SBD message successfully sent from the ISU to the ESS.
          2: An error occurred while attempting to send SBD message from ISU to ESS.
        
        <MT status>:
          The MT status provides an indication of the disposition of the mobile terminated transaction. The
          field can take on the following values:
          0: No SBD message to receive from the ESS.
          1: SBD message successfully received from the ESS.
          2: An error occurred while attempting to perform a mailbox check or receive a message
          from the ESS.
     */
    // check MO status ( == 1 ? message sent. == 2 ? error; try again?)
    // check MT status ( == 1 ? message received; now call SBDRT.)
  }
}

// Initialize all private variables
void variableInit()
{
	
}
