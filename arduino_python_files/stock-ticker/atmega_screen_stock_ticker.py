#!/usr/bin/env python

import urllib, json

# Arduino satff
import serial

class Stock_ticker(object):

    def __init__(self):
        self.GOOG_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22GOOG%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json' # The URL for yahoo stock rest api for google stock. No need for an account
        
        self.AAPL_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22AAPL%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json' # The URL for yahoo stock rest api for apple stock. No need for an account
        
        self.TWTR_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22TWTR%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json'# The URL for yahoo stock rest api for twitter stock. No need for an account
        
        self.FB_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22FB%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json' # The URL for yahoo stock rest api for facebook stock. No need for an account
    
    def GOOG_stock_pull(self):
        GOOG_url_open = urllib.urlopen(self.GOOG_url_GBP) # Opens the yahoo rest api URL for that stock
        GOOG_url_read = GOOG_url_open.read() # Reads the yahoo rest api URL for that stock
        parsed_json = json.loads(GOOG_url_read) # Parses the json from the yahoo rest api URL
        self.GOOG_price = parsed_json['query']['results']['quote']['LastTradePriceOnly'] # Parses the last trade price from that stock
        self.GOOG_percent = parsed_json['query']['results']['quote']['PercentChange'] # Parses the percent change from that stock
    
    def AAPL_stock_pull(self):
        AAPL_url_open = urllib.urlopen(self.AAPL_url_GBP) # Opens the yahoo rest api URL for that stock
        AAPL_url_read = AAPL_url_open.read() # Reads the yahoo rest api URL for that stock
        parsed_json = json.loads(AAPL_url_read) # Parses the json from the yahoo rest api URL
        self.AAPL_price = parsed_json['query']['results']['quote']['LastTradePriceOnly'] # Parses the last trade price from that stock
        self.AAPL_percent = parsed_json['query']['results']['quote']['PercentChange'] # Parses the percent change from that stock
        
    def TWTR_stock_pull(self):
        TWTR_url_open = urllib.urlopen(self.TWTR_url_GBP) # Opens the yahoo rest api URL for that stock
        TWTR_url_read = TWTR_url_open.read() # Reads the yahoo rest api URL for that stock
        parsed_json = json.loads(TWTR_url_read) # Parses the json from the yahoo rest api URL
        self.TWTR_price = parsed_json['query']['results']['quote']['LastTradePriceOnly'] # Parses the last trade price from that stock
        self.TWTR_percent = parsed_json['query']['results']['quote']['PercentChange'] # Parses the percent change from that stock
        
    def FB_stock_pull(self):
        FB_url_open = urllib.urlopen(self.FB_url_GBP) # Opens the yahoo rest api URL for that stock
        FB_url_read = FB_url_open.read() # Reads the yahoo rest api URL for that stock
        parsed_json = json.loads(FB_url_read) # Parses the json from the yahoo rest api URL
        self.FB_price = parsed_json['query']['results']['quote']['LastTradePriceOnly'] # Parses the last trade price from that stock
        self.FB_percent = parsed_json['query']['results']['quote']['PercentChange'] # Parses the percent change from that stock


if __name__ == '__main__':
    try: # Checks to see if the USB0 is avalible if not it uses USB1
        arduino = serial.Serial('/dev/ttyUSB0', 9600) # Arduino Serial port
    except:
        arduino = serial.Serial('/dev/ttyUSB1', 9600) # Arduino Serial port
        
    update = Stock_ticker()
    update.GOOG_stock_pull()
    
    print "GOOG price %.2f %s" % (float(update.GOOG_price), update.GOOG_percent)
    arduino.write("#1#%.2f %s;" % (float(update.GOOG_price), update.GOOG_percent)) # Sends the stock price and percent change to the arduino
    first1, last1 = str(update.GOOG_percent).split("%", 1) # Splits the percent into the number and the percent sign
    print first1 # Number striped from the percent sign
    #print last1 # Just the percent sign
    
    if float(first1) > 0: # If the number is greater then 0 turn the RGB LED green
        print "Green"
        arduino.write("#2#1;") # Tell the arduino to turn the RGB LED green
        
    elif float(first1) < 0: # If the number is less then 0 then turn the RGB LED red
        print "Red"
        arduino.write("#2#2;") # Tell the arduino to turn the RGB LED red
        
    else: # Falls back to being even and turns the RGB LED off
        print "Even"
        arduino.write("#2#0;") # Tells the arduino to turn off the RGB LED
    
    update.AAPL_stock_pull()
    print "AAPL price %.2f %s" % (float(update.AAPL_price), update.AAPL_percent)
    arduino.write("#3#%.2f %s;" % (float(update.AAPL_price), update.AAPL_percent)) # Sends the stock price and percent change to the arduino
    first2, last2 = str(update.AAPL_percent).split("%", 1) # Splits the percent into the number and the percent sign
    print first2 # Number striped from the percent sign
    #print last2 # Just the percent sign
    
    if float(first2) > 0: # If the number is greater then 0 turn the RGB LED green
        print "Green"
        arduino.write("#4#1;") # Tell the arduino to turn the RGB LED green
        
    elif float(first2) < 0: # If the number is less then 0 then turn the RGB LED red
        print "Red"
        arduino.write("#4#2;") # Tell the arduino to turn the RGB LED red
        
    else: # Falls back to being even and turns the RGB LED off
        print "Even"
        arduino.write("#4#0;") # Tells the arduino to turn off the RGB LED
    
    update.TWTR_stock_pull()
    print "TWTR price %.2f %s" % (float(update.TWTR_price), update.TWTR_percent)
    arduino.write("#5#%.2f %s;" % (float(update.TWTR_price), update.TWTR_percent)) # Sends the stock price and percent change to the arduino
    first3, last3 = str(update.TWTR_percent).split("%", 1) # Splits the percent into the number and the percent sign
    print first3 # Number striped from the percent sign
    #print last3 # Just the percent sign
    
    if float(first3) > 0: # If the number is greater then 0 turn the RGB LED green
        print "Green"
        arduino.write("#6#1;") # Tell the arduino to turn the RGB LED green
        
    elif float(first3) < 0: # If the number is less then 0 then turn the RGB LED red
        print "Red"
        arduino.write("#6#2;") # Tell the arduino to turn the RGB LED red
        
    else: # Falls back to being even and turns the RGB LED off
        print "Even"
        arduino.write("#6#0;") # Tells the arduino to turn off the RGB LED
    
    update.FB_stock_pull()
    print "FB price %.2f %s" % (float(update.FB_price), update.FB_percent)
    arduino.write("#7#%.2f %s;" % (float(update.FB_price), update.FB_percent)) # Sends the stock price and percent change to the arduino
    first4, last4 = str(update.FB_percent).split("%", 1) # Splits the percent into the number and the percent sign
    print first4 # Number striped from the percent sign
    #print last4 # Just the percent sign
    
    if float(first4) > 0: # If the number is greater then 0 turn the RGB LED green
        print "Green"
        arduino.write("#8#1;") # Tell the arduino to turn the RGB LED green
        
    elif float(first4) < 0: # If the number is less then 0 then turn the RGB LED red
        print "Red"
        arduino.write("#8#2;") # Tell the arduino to turn the RGB LED red
        
    else: # Falls back to being even and turns the RGB LED off
        print "Even"
        arduino.write("#8#0;") # Tells the arduino to turn off the RGB LED
