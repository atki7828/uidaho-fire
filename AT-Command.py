#!/usr/bin/python

# allows user to send AT commands directly to iridium modem.
# 

import serial
import re
import time
#from threading import Thread

'''
    note:  iridium 9523 has autobaud.
    I have tested it with baud = 9600, 19200, and 115200, 
    and all three seem to work fine.
'''

print "enter AT command."
print "type \'exit\' to exit"
print "type \'check\' to check for new messages\n"
iridium = serial.Serial("/dev/ttyS0",baudrate=9600,timeout=5)
#iridium = serial.Serial("/dev/ttyUSB0",baudrate=19200,timeout=5)

iridium.reset_input_buffer()

def WriteCommand():
    command = raw_input("Enter command: ")
    if(command == 'exit'):
        exit()
    if(command == 'check'):
        CheckForMessages()
    else:
        iridium.write(command + "\r\n")

def CheckForMessages():
    iridium.flush()
    response = "" 
    while(iridium.in_waiting):
        response += iridium.read()
    if(response != "" and re.search('[^\r\n]',response)): 
        print "raw: " + repr(response)
        print "escaped: " + response + '\n'


while 1:
    WriteCommand()
    #Thread(target = CheckForMessages()).start()

    '''
    command = raw_input("Enter AT Command: ")
    if command == 'exit':
        exit()
    iridium.write(command + "\r\n")
    time.sleep(2)
    response = ''
    while iridium.in_waiting:# and ('OK' not in response and 'ERROR' not in response): 
        response += iridium.read()
    print(response)

    iridium.flush()
    '''



