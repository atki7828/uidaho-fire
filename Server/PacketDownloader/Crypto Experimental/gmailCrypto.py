#!/usr/bin/python

# created using code from:
# https://developers.google.com/gmail/api/quickstart/python
# and https://developers.google.com/gmail/api/v1/reference/users/messages/attachments/get#python
# and https://developers.google.com/gmail/api/v1/reference/users/messages/list#python

''' requires:
        google-api-python-client 
        google-auth-httplib2 
        google-auth-oauthlib

        install modules with:
            > pip install [module-name]
        may require pip:
            > sudo apt install python-pip
'''

'''
usage:
    import gmail

    service = gmail.get_credentials()
    messages = NewMessages(service)
    for message in messages:
        GetAttachment(service,message['id'])
'''

#TODO:
'''
    has to mark a downloaded e-mail as 'read' such that we don't redownload e-mails over and over
    see if gmail api has a way to query if there are any new messages, or just get an 'unread messages' count, something like that.
    find a better way to handle the user authorization, because this is messy
'''
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
import pickle
import os.path
import base64
from Crypto import PythonCryptoWrapper

isEncryptionEnabled = true

'''
 authorization scope.  we want modify, to both read and mark as read..  
 see other scopes here:  https://developers.google.com/gmail/api/auth/scopes
 '''
scopes = ['https://www.googleapis.com/auth/gmail.modify']


"""
"""
creds = None
creds_dir = './.credentials'
creds_file = 'credentials.json'
creds_path = os.path.join(creds_dir,creds_file)
packets_dir = './packets'
if(not os.path.exists(packets_dir)):
    os.makedirs(packets_dir)

# The file token.pickle stores the user's access and refresh tokens, and is
# created automatically when the authorization flow completes for the first
# time.


'''NOTE
    gmail's API now requires users to authorize through a browser, so running this entirely from our command line won't work.
    so first, you have to first run the get_credentials() program on a regular computer (with a browser), 
    it will have you verify through your browser to generate the credentials token...
    and then you have to upload the token on to the server.
    very roundabout way of doing it.
    i'm like 90% sure there's a better way.
'''


# after user has authorized themselves,
# this function will verify credentials and return a service object to be used to query gmail.
def get_credentials():
    creds = None
    if not os.path.exists(creds_dir):
        os.makedirs(creds_dir)
    
    if os.path.exists('token.pickle'):
            with open('token.pickle','rb') as token:
                creds = pickle.load(token)
    if not creds or not creds.valid:
        if creds and creds.expired and creds.refresh_token:
            creds.refresh(Request())
        else:
            flow = InstalledAppFlow.from_client_secrets_file(creds_path,scopes)
            creds = flow.run_local_server(port=0)
        with open('token.pickle','wb') as token:
            pickle.dump(creds,token)
    service = build('gmail','v1',credentials=creds)
    print('user authorized')
    return service
    
# using service object returned in previous function,
# this returns the message IDs of all unread e-mails from iridium that have an attachment.
# we will also want to mark every message as 'read' after the attachment is downloaded.
def NewMessages(service):
    print('querying inbox')
    response = service.users().messages().list(userId='me',q='from:sbdservice@sbd.iridium.com is:unread has:attachment').execute()

    messages = []
    if('messages' in response):
        messages.extend(response['messages'])
    while 'nextPageToken' in response:
        page_token = response['nextPageToken']
        response = service.users().messages().list(userId='me',q='from:sbdservice@sbd.iridium.com is:unread',pageToken=page_token).execute()
        messages.extend(response['messages'])
    return messages

# take a message's ID, and download the attachment.
# stores it in a packets/ directory.
def GetAttachment(service, msg_id):
    message = service.users().messages().get(userId='me',id=msg_id).execute()

    # this downloads body of email- not SBD packet itself:
    for part in message['payload']['parts']:
        if(part['mimeType'] == 'text/plain'):
            bodyData = base64.urlsafe_b64decode(part['body']['data'].encode('UTF-8'))
            # Wrong spot - Move it somewhere else
            if(isEncryptionEnabled):
                crypto = Crypto()
                plaintext = crypto.encrypt_cbc(bodyData)
        print(part)

        # this is the actual SBD packet download:
        if part['filename']:
            attId = part['body']['attachmentId']
            attachment = service.users().messages().attachments().get(userId='me',messageId=msg_id,id=attId).execute()
            attData = base64.urlsafe_b64decode(attachment['data'].encode('UTF-8'))
            path = os.path.join(packets_dir,part['filename'])
            with open(path,'w') as f:
                f.write(attData)
            if(bodyData):
                path = os.path.join(packets_dir,part['filename']+'.txt')
                with open(path,'w') as f:
                    f.write(bodyData)
            print('message downloaded to '+str(path))

    service.users().messages().modify(userId='me',id=msg_id,body={'removeLabelIds':['UNREAD'],'addLabelIds':[]}).execute()

