#!/usr/bin/python

import gmail

service = gmail.get_credentials()

messages = gmail.NewMessages(service)

if(len(messages) > 0):
    for msg in messages:
        gmail.GetAttachment(service,msg['id'])
