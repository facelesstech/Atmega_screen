#!/usr/bin/env python

import urllib2, json, time, datetime
from time import strftime

# Arduino stuff
import serial

class Ice(object):

    def __init__(self):
        self.start_wipe_time_lowest = datetime.time(13, 00) 
        self.end_wipe_time_lowest = datetime.time(13, 20)
        self.start_wipe_time_highest = datetime.time(23,25)
        self.end_wipe_time_highest = datetime.time(23, 50)
        self.now = datetime.datetime.now().time()
        #self.now = datetime.time(12, 00) # test
        #self.temp_dict = {} # test
        #self.temp_c = 11.6   # Test
        #self.location = 'chester' # test
        
    def lowest_temp_read(self):
        load_lowest_temp = open("lowest_temp.txt") # Opens the file
        self.read_lowest_temp = load_lowest_temp.read() # Reads the file
        
    def write_lowest_temp(self):
        load_lowest_temp_write = open("lowest_temp.txt", 'w') # Opens the file and allows you to write to it
        load_lowest_temp_write.write('%s' % self.temp_c) # Writes the data to the file
        load_lowest_temp_write.close() # Closes the file
        
    def read_lowest_time(self):
        load_lowest_time = open("lowest_time.txt") # Opens the file
        self.read_lowest_time = load_lowest_time.read() # Reads the file
 
    def write_lowest_time(self):
        load_lowest_time_write = open("lowest_time.txt", 'w') # Opens the file and allows you to write to it
        load_lowest_time_write.write('%s' % strftime('%H:%M')) # Writes the data to the file
        load_lowest_time_write.close() # Closes the file
           
    def read_highest_temp(self):
        load_highest_temp = open("highest_temp.txt") # Opens the file
        self.read_highest_temp = load_highest_temp.read() # Reads the file
        
    def write_highest_temp(self):
        load_highest_temp_write = open("highest_temp.txt", 'w') # Opens the file and allows you to write to it
        load_highest_temp_write.write('%s' % self.temp_c) # Writes the data to the file
        load_highest_temp_write.close() # Closes the file
        
    def read_highest_time(self):
        load_highest_time = open("highest_time.txt") # Opens the file
        self.read_highest_time = load_highest_time.read() # Reads the file

    def write_highest_time(self):
        load_highest_time_write = open("highest_time.txt", 'w') # Opens the file and allows you to write to it
        load_highest_time_write.write('%s' % strftime('%H:%M')) # Writes the data to the file
        load_highest_time_write.close() # Closes the file
    
    def checking(self):
        # You will have to registor at wunderground here http://www.wunderground.com/weather/api/ and replace the xxxxxxxxxxxxxxxx below with your API key
        f = urllib2.urlopen('http://api.wunderground.com/api/xxxxxxxxxxxxxxxx/geolookup/conditions/q/UK/Chester.json') # Opens the json link
        json_string = f.read() # Reads the json
        parsed_json = json.loads(json_string) # parses the json
        self.location = parsed_json['location']['city'] # Parse the location and city ie UK and chester
        self.temp_c = parsed_json['current_observation']['temp_c'] # Parse the temperature C for your location
        self.weather = parsed_json['current_observation']['weather'] # Parse weather condition for your location
        self.feels = parsed_json['current_observation']['feelslike_c'] # Parse feels like temperature C for your location
        self.wind = parsed_json['current_observation']['wind_mph'] # Parse wind speed for your location
        f.close() 
        
if __name__ == '__main__':
    scrap = Ice() # Load the class
    scrap.checking() 
    scrap.lowest_temp_read()
    scrap.read_highest_temp()
    try: # Checks to see if the USB0 is avalible if not it uses USB1
        arduino = serial.Serial('/dev/ttyUSB0', 9600) # Arduino Serial port
    except:
        arduino = serial.Serial('/dev/ttyUSB1', 9600) # Arduino Serial port
    #except:
        #arduino = serial.Serial('/dev/ttyUSB2', 9600) #arduino nano
    #print arduino.readline()
    print '''
          _                              
     /\  | |                             
    /  \ | |_ _ __ ___   ___  __ _  __ _ 
   / /\ \| __| '_ ` _ \ / _ \/ _` |/ _` |
  / ____ \ |_| | | | | |  __/ (_| | (_| |
 /_/____\_\__|_| |_| |_|\___|\__, |\__,_|
  / ____|                     __/ |      
 | (___   ___ _ __ ___  ___ _|___/       
  \___ \ / __| '__/ _ \/ _ \ '_ \        
  ____) | (__| | |  __/  __/ | | |       
 |_____/ \___|_|  \___|\___|_| |_|       
                                      
Version beta 1.0
Atmega_screen edition
''' 
    scrap.checking()
    if scrap.start_wipe_time_lowest < scrap.now < scrap.end_wipe_time_lowest: # If the time falls between the two set times it will write the current temp to the lowest temp file
        scrap.write_lowest_temp() # Write current temp to file
        scrap.write_lowest_temp() # Write current time to file
    else:
        pass # Else do nothing
        
    if scrap.start_wipe_time_highest < scrap.now < scrap.end_wipe_time_highest: # If the time falls between the two set times it will write the current temp to the highest temp file
        scrap.write_highest_temp() # Write current temp to file
        scrap.write_highest_time() # Write current time to file
    else:
        pass # Else do nothing
        
    print strftime('%H:%M:%S') # Print time
    print 'Current location\n%s' % scrap.location 
    print 'Current temp\n%sc' % scrap.temp_c
    print 'Feels like\n%sc' % scrap.feels
    print 'Weaher\n%s' % scrap.weather
    print 'Wind speed\n%sMph' % scrap.wind
    
    arduino.write('#3%s;' % scrap.weather) # Sends weather conditions to the arduino
    arduino.write('#1%s;' % scrap.temp_c) # Sends temp C to the arduino
    arduino.write('#2%s;' % scrap.feels) # Sends feels like temp C to the arduino
    arduino.write('#5%sMph;' % scrap.wind) # Sends wind speed to the arduino
       
    if float(scrap.temp_c) > float(scrap.read_lowest_temp): # Checks to see if the current temp is higher then the temp in lowest_temp.txt and if it is it sends the data to the arduino and over writes the lowest_temp.txt file
        scrap.read_lowest_time() # Reads the lowest_time.txt
        print "Current temp is not lower"
        print "Lower temp\n%s" % scrap.read_lowest_temp
        arduino.write('#4%s;' % scrap.read_lowest_temp) # Sends lowest_temp.txt to the arduino
        arduino.write('#6%s;' % scrap.read_lowest_time) # Sends lowest_time.txt to the arduino
        
    elif float(scrap.temp_c) < float(scrap.read_lowest_temp): # Checks to see if the current temp is lower then the temp in lowest_temp.txt and if it is it sends the data to the arduino and over writes lowest_temp.txt
        print "Current temp is lower"
    	print "Lower temp\n%s" % scrap.temp_c
        scrap.write_lowest_temp() # Writes the temp to lowest_temp.txt
        arduino.write('#4%s;' % scrap.temp_c) # Sends current temp to the arduino
        arduino.write('#6%s;' % strftime('%H:%M')) # Sends current time to the arduino
        scrap.write_lowest_time() # Writes the current time to lowest_time.txt
        
    #elif float(scrap.read_lowest_temp) < float(-0.1): # Not currently used. 
        #arduino.write('blue@') # Make the RGB led turn blue
        
    else: # If non of the previous stats have been meet then it sends the current time and temp to the arduino
        print "Same temp"
        print "Lower temp\n%s" % scrap.temp_c
        arduino.write('#4%s;' % scrap.temp_c) # Sends current temp to the arduino
        arduino.write('#6%s;' % strftime('%H:%M')) # Sends current time to the arduino
        scrap.write_lowest_time() # Writes the current time to lowest_time.txt
        
    scrap.lowest_temp_read() # Reads the lowest_temp.txt file
    
    #if float(scrap.read_lowest_temp) < 0: # Not currently used
        #arduino.write('blue@') # Make the RGB led turn blue
        #print 'blue'
    #else:
        #print 'not blue'
        
    if float(scrap.temp_c) < float(scrap.read_highest_temp): # Checks to see if current temp is lower then highest_temp.txt. If it is lower it sends highest_temp.txt temp to arduino
        scrap.read_highest_time()
        print "Current temp is not higher"
        print "Higher temp\n%s" % scrap.read_highest_temp
        arduino.write('#7%s;' % scrap.read_highest_temp) # Sends highest_temp.txt to the arduino
        arduino.write('#8%s;' % scrap.read_highest_time) # Sends highest_time.txt to the arduino
        
    elif float(scrap.temp_c) > float(scrap.read_highest_temp): # Checks to see if the current temp is higher then highest_temp.txt. If it is it sends current temp to arduino
        print "Current temp is higher"
        print "Highest temp\n%s" % scrap.temp_c
        scrap.write_highest_temp() # Writes current temp to highest_temp.txt
        arduino.write('#7%s;' % scrap.temp_c) # Sends current temp to arduino
        arduino.write('#8%s;' % strftime('%H:%M')) # Sends current time to arduino
        scrap.write_highest_time() # Writes current time to highest_time.txt
        
    else: # If non of the previous stats have been meet then it sends the current time and temp to the arduino
        print "Same temp"
        print "Highest temp\n%s" % scrap.temp_c
        arduino.write('#7%s;' % scrap.temp_c) # Sends current temp to arduino
        arduino.write('#8%s;' % strftime('%H:%M')) # Sends current time to arduino
        scrap.write_highest_time() # Writes the current time to highest_time.txt
        
        
    if 0 < scrap.temp_c <= 10: # Not currently used
        print 'aqua'
        #arduino.write('aqua@') # Change RGB LED to aqua
    elif 10 < scrap.temp_c <= 20: # Not currently used
        print 'yellow'
        #arduino.write('yellow@') # Change RGB LED to yellow
    elif 20 < scrap.temp_c <= 40: # Not currently used
        print 'red'
        #arduino.write('red@') # Change RGB LED to red
    else: # Not currently used
        print 'not colour'
