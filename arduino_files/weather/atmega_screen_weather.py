#!/usr/bin/env python

import urllib2, json, time, datetime
from time import strftime

# Arduino stuff
import serial

class Ice(object):

    def __init__(self):
        self.secs = 900
        self.start_wipe_time_lowest = datetime.time(13, 00)
        self.end_wipe_time_lowest = datetime.time(13, 20)
        self.start_wipe_time_highest = datetime.time(23,25)
        self.end_wipe_time_highest = datetime.time(23, 50)
        self.now = datetime.datetime.now().time()
        #self.now = datetime.time(12, 00) # test
        #self.temp_dict = {}
        #self.temp_c = 11.6   # Test
        #self.location = 'chester' # test
        
    def lowest_temp_read(self):
        load_lowest_temp = open("lowest_temp.txt")
        self.read_lowest_temp = load_lowest_temp.read()
        
    def write_lowest_temp(self):
        load_lowest_temp_write = open("lowest_temp.txt", 'w')
        load_lowest_temp_write.write('%s' % self.temp_c)
        load_lowest_temp_write.close()
        
    def read_lowest_time(self):
        load_lowest_time = open("lowest_time.txt")
        self.read_lowest_time = load_lowest_time.read()

    def write_lowest_time(self):
        load_lowest_time_write = open("lowest_time.txt", 'w')
        load_lowest_time_write.write('%s' % strftime('%H:%M'))
        load_lowest_time_write.close()
           
    def read_highest_temp(self):
        load_highest_temp = open("highest_temp.txt")
        self.read_highest_temp = load_highest_temp.read()
        
    def write_highest_temp(self):
        load_highest_temp_write = open("highest_temp.txt", 'w')
        load_highest_temp_write.write('%s' % self.temp_c)
        load_highest_temp_write.close()
        
    def read_highest_time(self):
        load_highest_time = open("highest_time.txt")
        self.read_highest_time = load_highest_time.read()

    def write_highest_time(self):
        load_highest_time_write = open("highest_time.txt", 'w')
        load_highest_time_write.write('%s' % strftime('%H:%M'))
        load_highest_time_write.close()
    
    def checking(self):
        f = urllib2.urlopen('http://api.wunderground.com/api/080b28233d8a1a49/geolookup/conditions/q/UK/Chester.json')
        json_string = f.read()
        parsed_json = json.loads(json_string)
        self.location = parsed_json['location']['city']
        self.temp_c = parsed_json['current_observation']['temp_c']
        self.weather = parsed_json['current_observation']['weather']
        self.feels = parsed_json['current_observation']['feelslike_c']
        self.wind = parsed_json['current_observation']['wind_mph']
        f.close()
        
if __name__ == '__main__':
    scrap = Ice()
    scrap.checking()
    scrap.lowest_temp_read()
    scrap.read_highest_temp()
    try:
        arduino = serial.Serial('/dev/ttyUSB0', 9600) #arduino nano
    except:
        arduino = serial.Serial('/dev/ttyUSB1', 9600) #arduino nano
    #except:
        #arduino = serial.Serial('/dev/ttyUSB2', 9600) #arduino nano
    #print arduino.readline()
    print '''
 _    _            _   _                      
| |  | |          | | | |                     
| |  | | ___  __ _| |_| |__   ___ _ __        
| |/\| |/ _ \/ _` | __| '_ \ / _ \ '__|       
\  /\  /  __/ (_| | |_| | | |  __/ |          
 \/  \/ \___|\__,_|\__|_| |_|\___|_|          
                                              
                                              
                 _   _____                    
                | | |_   _|                   
  __ _ _ __   __| |   | | ___ _ __ ___  _ __  
 / _` | '_ \ / _` |   | |/ _ \ '_ ` _ \| '_ \ 
| (_| | | | | (_| |   | |  __/ | | | | | |_) |
 \__,_|_| |_|\__,_|   \_/\___|_| |_| |_| .__/ 
                                       | |    
                                       |_|  
Version alpha 0.8
Arduino screen edition!!!!!!!! (now with button control and RGB LED)
''' 
    scrap.checking()
    if scrap.start_wipe_time_lowest < scrap.now < scrap.end_wipe_time_lowest:
        scrap.write_lowest_temp()
    else:
        pass
        
    if scrap.start_wipe_time_highest < scrap.now < scrap.end_wipe_time_highest:
        scrap.write_highest_temp()
    else:
        pass
        
    print strftime('%H:%M:%S')
    print 'Current location\n%s' % scrap.location
    print 'Current temp\n%sc' % scrap.temp_c
    print 'Feels like\n%sc' % scrap.feels
    print 'Weaher\n%s' % scrap.weather
    print 'Wind speed\n%sMph' % scrap.wind
        
    arduino.write('#3%s;' % scrap.weather)
    arduino.write('#1%s;' % scrap.temp_c)
    arduino.write('#2%s;' % scrap.feels)
    arduino.write('#5%sMph;' % scrap.wind)
       
    if float(scrap.temp_c) > float(scrap.read_lowest_temp):
        scrap.read_lowest_time()
        print "Current temp is not lower"
        print "Lower temp\n%s" % scrap.read_lowest_temp
        arduino.write('#4%s;' % scrap.read_lowest_temp)
        arduino.write('#6%s;' % scrap.read_lowest_time)
        
    elif float(scrap.temp_c) < float(scrap.read_lowest_temp):
        print "Current temp is lower"
    	print "Lower temp\n%s" % scrap.temp_c
        scrap.write_lowest_temp()
        arduino.write('#4%s;' % scrap.temp_c)
        arduino.write('#6%s;' % strftime('%H:%M'))
        scrap.write_lowest_time()
        
    elif float(scrap.read_lowest_temp) < float(-0.1):
        arduino.write('blue@')
        
    else:
        print "Same temp"
        print "Lower temp\n%s" % scrap.temp_c
        arduino.write('#4%s;' % scrap.temp_c)
        arduino.write('#6%s;' % strftime('%H:%M'))
        scrap.write_lowest_time()
        
    scrap.lowest_temp_read() 
    
    if float(scrap.read_lowest_temp) < 0:
        #arduino.write('blue@')
        print 'blue'
    else:
        print 'not blue'
        
    if float(scrap.temp_c) < float(scrap.read_highest_temp):
        scrap.read_highest_time()
        print "Current temp is not higher"
        print "Higher temp\n%s" % scrap.read_highest_temp
        arduino.write('#7%s;' % scrap.read_highest_temp)
        arduino.write('#8%s;' % scrap.read_highest_time)
        
    elif float(scrap.temp_c) > float(scrap.read_highest_temp):
        print "Current temp is higher"
        print "Highest temp\n%s" % scrap.temp_c
        scrap.write_highest_temp()
        arduino.write('#7%s;' % scrap.temp_c)
        arduino.write('#8%s;' % strftime('%H:%M'))
        scrap.write_highest_time()
        
    else:
        print "Same temp"
        print "Highest temp\n%s" % scrap.temp_c
        arduino.write('#7%s;' % scrap.temp_c)
        arduino.write('#8%s;' % strftime('%H:%M'))
        scrap.write_highest_time()
        
        
    if 0 < scrap.temp_c <= 10:
        print 'aqua'
        #arduino.write('aqua@')
    elif 10 < scrap.temp_c <= 20:
        print 'yellow'
        #arduino.write('yellow@')
    elif 20 < scrap.temp_c <= 40:
        print 'red'
        #arduino.write('red@')
    else:
        print 'not colour'
    #time.sleep(scrap.secs)
