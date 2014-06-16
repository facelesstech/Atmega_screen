#!/usr/bin/env python

import urllib, json

# Arduino satff
import serial

class Stock_ticker(object):

    def __init__(self):
        self.GOOG_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22GOOG%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json'
        
        self.AAPL_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22AAPL%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json'
        
        self.TWTR_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22TWTR%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json'
        
        self.FB_url_GBP = 'http://query.yahooapis.com/v1/public/yql?q=select%20*%20from%20yahoo.finance.quotes%20where%20symbol%20in%20(%22FB%22)%0A%09%09&env=http%3A%2F%2Fdatatables.org%2Falltables.env&format=json'
    
    def GOOG_stock_pull(self):
        GOOG_url_open = urllib.urlopen(self.GOOG_url_GBP)
        GOOG_url_read = GOOG_url_open.read()
        parsed_json = json.loads(GOOG_url_read)
        self.GOOG_price = parsed_json['query']['results']['quote']['LastTradePriceOnly']
        self.GOOG_percent = parsed_json['query']['results']['quote']['PercentChange']
    
    def AAPL_stock_pull(self):
        AAPL_url_open = urllib.urlopen(self.AAPL_url_GBP)
        AAPL_url_read = AAPL_url_open.read()
        parsed_json = json.loads(AAPL_url_read)
        self.AAPL_price = parsed_json['query']['results']['quote']['LastTradePriceOnly']
        self.AAPL_percent = parsed_json['query']['results']['quote']['PercentChange']
        
    def TWTR_stock_pull(self):
        TWTR_url_open = urllib.urlopen(self.TWTR_url_GBP)
        TWTR_url_read = TWTR_url_open.read()
        parsed_json = json.loads(TWTR_url_read)
        self.TWTR_price = parsed_json['query']['results']['quote']['LastTradePriceOnly']
        self.TWTR_percent = parsed_json['query']['results']['quote']['PercentChange']
        
    def FB_stock_pull(self):
        FB_url_open = urllib.urlopen(self.FB_url_GBP)
        FB_url_read = FB_url_open.read()
        parsed_json = json.loads(FB_url_read)
        self.FB_price = parsed_json['query']['results']['quote']['LastTradePriceOnly']
        self.FB_percent = parsed_json['query']['results']['quote']['PercentChange']


if __name__ == '__main__':
    arduino = serial.Serial('/dev/ttyUSB0', 9600) #arduino nano
    #arduino = serial.Serial('/dev/ttyUSB1', 9600) #arduino nano
    update = Stock_ticker()
    
    update.GOOG_stock_pull()
    print "GOOG price %s %s" % (update.GOOG_price, update.GOOG_percent)
    arduino.write("%s %sA" % (update.GOOG_price, update.GOOG_percent))
    first1, last1 = str(update.GOOG_percent).split("%", 1)
    print first1
    #print last1
    if float(first1) > 0:
        print "Green"
        arduino.write("1a")
        
    elif float(first1) < 0:
        print "Red"
        arduino.write("2a")
        
    else:
        print "Even"
        arduino.write("0a")
    
    update.AAPL_stock_pull()
    print "AAPL price %s %s" % (update.AAPL_price, update.AAPL_percent)
    arduino.write("%s %sB" % (update.AAPL_price, update.AAPL_percent))
    first2, last2 = str(update.AAPL_percent).split("%", 1)
    print first2
    if float(first2) > 0:
        print "Green"
        arduino.write("1b")
        
    elif float(first2) < 0:
        print "Red"
        arduino.write("2b")
        
    else:
        print "Even"
        arduino.write("0b")
    
    update.TWTR_stock_pull()
    print "TWTR price %s %s" % (update.TWTR_price, update.TWTR_percent)
    arduino.write("%s %sC" % (update.TWTR_price, update.TWTR_percent))
    first3, last3 = str(update.TWTR_percent).split("%", 1)
    print first3
    if float(first3) > 0:
        print "Green"
        arduino.write("1c")
        
    elif float(first3) < 0:
        print "Red"
        arduino.write("2c")
        
    else:
        print "Even"
        arduino.write("0c")
    
    update.FB_stock_pull()
    print "FB price %s %s" % (update.FB_price, update.FB_percent)
    arduino.write("%s %sD" % (update.FB_price, update.FB_percent))
    first4, last4 = str(update.FB_percent).split("%", 1)
    print first4
    if float(first4) > 0:
        print "Green"
        arduino.write("1d")
        
    elif float(first4) < 0:
        print "Red"
        arduino.write("2d")
        
    else:
        print "Even"
        arduino.write("0d")
