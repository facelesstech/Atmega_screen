#!/usr/bin/env python
# -*- coding: utf-8 -*- # Added this to displace the pound sign
import urllib, json

# Arduino stuff
import serial

class Currency_ticker(object):

    def __init__(self):
        # sign up for a API key here http://currency-api.appspot.com/ and replace xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx with it
        self.url_USD = "http://currency-api.appspot.com/api/USD/GBP.json?key=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        # sign up for a API key here http://currency-api.appspot.com/ and replace xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx with it
        self.url_EUR = "http://currency-api.appspot.com/api/EUR/GBP.json?key=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
        # No need for api key to use this url
        self.bitcoin_url_GBP = 'https://api.bitcoinaverage.com/ticker/global/GBP/'
    
    def GBP_bitcoin_convert(self):
        bitcoin_url_open = urllib.urlopen(self.bitcoin_url_GBP) # Open the API url
        bitcoin_url_read = bitcoin_url_open.read() # Read the API url
        parsed_json = json.loads(bitcoin_url_read) # Parse the json
        self.one_bitcoin = parsed_json['ask'] # Bitcoin price
    
    def USD_convert(self):
        url_open_USD = urllib.urlopen(self.url_USD) # Open the API url
        result_USD = url_open_USD.read() # Read the API url
        results_USD = json.loads(result_USD) # Parse the json
        self.rate_USD = results_USD['rate'] # USD price
    
    def EUR_convert(self):
        url_open_EUR = urllib.urlopen(self.url_EUR) # Open the API url
        result_EUR = url_open_EUR.read() # Read the API url
        results_EUR = json.loads(result_EUR) # Parse the json
        self.rate_EUR = results_EUR['rate'] # EUR price

if __name__ == '__main__':
    try: # Checks to see if the USB0 is avalible if not it uses USB1
        arduino = serial.Serial('/dev/ttyUSB0', 9600) # Arduino Serial port
    except:
        arduino = serial.Serial('/dev/ttyUSB1', 9600) # Arduino Serial port
        
    update = Currency_ticker()
    update.USD_convert() # Run the pull script
    print "1 USD is worth £%.2f GPB" % update.rate_USD
    arduino.write('#1#%.2f;' % update.rate_USD) # Send rate to arduino
    update.EUR_convert() # Run the pull script
    print "1 EUR is worth £%.2f GPB" % update.rate_EUR
    arduino.write('#2#%.2f;' % update.rate_EUR) # Send rate to arduino
    update.GBP_bitcoin_convert() # Run the pull script
    print "1 Bitcoin is worth £%s" % update.one_bitcoin
    arduino.write('#3#%s;' % update.one_bitcoin) # Send rate to arduino
