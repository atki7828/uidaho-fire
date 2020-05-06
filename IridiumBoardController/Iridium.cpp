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

bool Iridium::ready() { return this->commState == IDLE; }

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
  this->SwitchState(WRITING);
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
  SerialUSB.print("dialing gateway: ");
  SerialUSB.println(GatewayNumber);
  this->write("ATDP");
  this->write(GatewayNumber);
  this->write("\r\n");
  this->SwitchState(DIALING);
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
  this->SwitchState(IDLE);
}

// take an action based on iridium's response to our commands.
void Iridium::ProcessResponse(String response) {

  String messageHolder[20];

  int responseSize = response.length();
  int numMessages = 0;
  for (int i = 0; i < responseSize; i++){
    if(response[i] != '\r'){
      String message = "";
      while(response[i] != '\r'){
        message += response[i++];
      }
      i++;
      messageHolder[numMessages++] = message;
    }else{
      i++;
    }
  }

  for(int i = 0; i < numMessages; i++) {
    if(messageHolder[i].indexOf("OK") > -1) {
      SerialUSB.println("Got OK");
      switch(this->commState) {
        case WRITING:
          this->InitiateSession();
          this->SwitchState(INITIATING);
          break;
        default:
          this->commState = IDLE;
          break;
      }
    }
    else if(messageHolder[i].indexOf("SBDRT:") > -1) {
      SerialUSB.println("Got SBDRT");
      // response was:  "+SBDRT:\r\n(incoming message)\r\n".
      // we need to store "(incoming message)" in it's own variable, and...do something with it.  maybe just print to SerialUSB for now.
      SerialUSB.println("MT message: " + messageHolder[i+1]);
      // According to Ames, anything incoming should go directly to TES...something like:  TESSer.write(messageHolder[i+1]);
      i++;
      this->SwitchState(IDLE);
    }
    else if(messageHolder[i].indexOf("SBDI:") > -1) {
      SerialUSB.println("Got SBDI");
      String MO, MT;
      int pos = messageHolder[i].indexOf(':') + 2;
      MO = messageHolder[i]][pos];
      pos+=2;
      while(messageHolder[i][pos] != ','){
        pos++;
      }
      pos+=2;
      MT = messageHolder[i][pos];

      if (MO == '2' || MT == '2'){
        SerialUSB.println("Error with SBD message");
        SerialUSB.println("Issuing SBDI command");
        this->write("AT+SBDI\r\n");
      }else if (MT == '1'){
        SerialUSB.println("Issuing SBDRT command");
        this->write("AT+SBDRT\r\n");
      }else{
        SerialUSB.println("SBDI successful");
        this->commState = IDLE;
      }
      this->SwitchState(IDLE);
    }
    else if(messageHolder[i].indexOf("CONNECT") > -1) {
      SerialUSB.println("Got CONNECT");
      this->SwitchState(CONNECTED);
      SerialUSB.println("Dial-up connection established successfully");
    }
  else if(messageHolder[i].indexOf("NO CARRIER") > -1) {
    SerialUSB.println("Got No Carrier");
    SerialUSB.println("Dial-up connection dropped");
    this->SwitchState(IDLE);
  }
  }

  return;
}

  void Iridium::SwitchState(communicationState state) {
    SerialUSB.println("state switched from " + statename(this->commState) + " to " + statename(state));
    this->commState = state;
  }

// just for debugging
String Iridium::statename(communicationState state) {
  switch(state) {
    case IDLE:
      return "IDLE";
    case WRITING:
      return "WRITING";
    case READING:
      return "READING";
    case INITIATING:
      return "INITIATING";
    case DIALING:
      return "DIALING";
    case CONNECTED:
      return "CONNECTED";
    default:
      return "";
  }
}

// Initialize all private variables
void variableInit()
{

}
