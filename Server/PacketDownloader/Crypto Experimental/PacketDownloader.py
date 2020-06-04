#!/usr/bin/python

import gmail
import time

service = gmail.get_credentials()


while 1:
    messages = gmail.NewMessages(service)

    if(len(messages) > 0):
        print(str(len(messages)) + ' messages waiting')
        for msg in messages:
            gmail.GetAttachment(service,msg['id'])
    time.sleep(5)
