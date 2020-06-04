# University of Idaho - Team FIRE

This software is developed by Team F.I.R.E. (Firebox Iridium Re-Entry) Computer Science students for the University of Idaho capstone course (Fall 2019 – Spring 2020). These Computer Science students include Michael Atkinson, Brandon Foss, and Lennin Rodriguez on the capstone project Suborbital Flight Communication with Battery Containment Unit sponsored by NASA Ames Research Center (NASA ARC). 

## Goal

The goal of this software is to program a microcontroller onboard a custom carrier module designed by Electrical Engineering students also on the capstone team to interact with incoming data from the TES (Tech Ed Satellite) and use an onboard Iridium-9523 to send data to a ground-based server. The data transmission should include options for both SBD (short burst data) for single messages sent at set intervals and dial-up for real time data transmission. A cryptosystem should also be implemented to provide transmission security throughout. 

The following activity diagram displays how the system should work to apply all of these goals within our system cycle. The actual code provided here accomplishes the majority of this system aside from the areas listed in the Notes section below.

![Data Flow Diagram that shows how the system should work to accomplish all goals.](https://github.com/atki7828/uidaho-fire/blob/master/Documents/FIRE%20-%20Activity%20Diagram%20V2%20-%20White%20Background.png?raw=true "Activity Data Flow Diagram")

The following class diagram shows our modular approach to accomplishing our goals listed above. The main class controls the Iridium-9523 functionality and the second class helps the Iridium accomplish security through AES-128 bit encryption and decryption functionality.

![Class Diagram that shows our modular design and how the cryptosystem works with the Iridium class.](https://github.com/atki7828/uidaho-fire/blob/master/Documents/FIRE%20-%20Class%20Diagram%20V3.png?raw=true "UML Class Diagram")

## Notes

Due to the hardware lack of a TCP/IP stack, the dial-up connection can be made but not utilized in our final code. Two options can be used to fix this problem, but we did not have the time to implement them into our final build. The first option is to implement a lightweight microprocessor-based TCP/IP stack such as lwIP. We attempted this solution for a while but discovered a better fix in switching the data service plan from a dial-up connection to a RUDICS based connection. This eliminates the need for a TCP/IP stack and provides additional benefits such as faster transfer rates and a more reliable connection. NASA ARC has been informed of this option but could not obtain the money necessary to switch the data plan before the end of our capstone project. This will have to be implemented by NASA ARC themselves once they do make the switch. 

The cryptosystem is implemented and works on a Windows basec computer. However, only encryption works on the board itself while attempting to use decryption causes the microprocessor to crash and restart. This will have to be fixed before it is ever used in a flight.

## Additional Information

For more information on our background research, goals, design choices, validation of a working program, and general information on what the rest of our capstone team completed, visit our Wikipedia page at [Wiki Page](http://mindworks.shoutwiki.com/wiki/NASA_Suborbital_Flight_Communication_and_Fire_Box).
