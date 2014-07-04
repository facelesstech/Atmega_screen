#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include "RTClib.h"

RTC_DS1307 rtc;

long waitUntiltime = 0;

String readString; // readstring is for collecting the serial buffer code
String temp_c;  // string that stores temp
String feels;  // string that stores feels like temp
String weather;  // string that stores weather forcast
String lowest;  // string that stores lowest temp
String wind;  // string that stores wind speed
String lowest_time;  // Time last lowest
String highest_temp;  // string that stores highest temp
String highest_time;   // string that stores highest temp time

// degree centigrade
byte degree[8] = {
  B01000,
  B10100,
  B01000,
  B00011,
  B00100,
  B00100,
  B00011,
  B00000
};

// Button stuff
const int button = 5;
int buttonPushCounter1 = 8;    // counts the button pushes
int buttonState1 = 0;    // tracks the button state
int lastButtonState1 = 0;    // last state of the button

// Debounce stuff
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Timing for the display cycle
long waitUntilcycle1 = 0;
long waitUntilcycle2 = 5000;
long waitUntilcycle3 = 10000;
long waitUntilcycle4 = 15000;
long waitUntilcycle5 = 20000;
long waitUntilcycle6 = 25000;
long waitUntilcycle7 = 30000;

// Sets the led pins
int redPin = 12;
int greenPin = 11;
int bluePin = 10;
//#define COMMON_ANODE

// RTC stuff
const int DS1307 = 0x68; // Address of DS1307 see data sheets
const char* days[] =
{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Initializes all values: 
byte second = 0;
byte minute = 0;
byte hour = 0;
byte weekday = 0;
byte monthday = 0;
byte month = 0;
byte year = 0;


void setup() {
  Wire.begin();
  lcd.backlight();
  lcd.init();
  lcd.begin(16, 2);
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);
  //lcd.createChar(0, degree);
  Serial.begin(9600);
  Serial.println("Waiting");
}

void loop() {
  //Serial.println(highest_temp);
  lcd.setCursor(0,0);
  if (Serial.available())  {
    char c = Serial.read();  //gets one byte from serial buffer
    
    if (c == ';') {
      if (readString.length() >0) {
        if (readString.startsWith("#1")) {
          temp_c = readString.substring(2);
          Serial.println(temp_c);
        }
        
        else if (readString.startsWith("#2")) {
          feels = readString.substring(2);
          Serial.println(feels);
        }
        
        else if (readString.startsWith("#3")) {
          weather = readString.substring(2);
          Serial.println(weather);
        }
        
        else if (readString.startsWith("#4")) {
          lowest = readString.substring(2);
          Serial.println(lowest);
        }
        
        else if (readString.startsWith("#5")) {
          wind = readString.substring(2);
          Serial.println(wind);
        }
        
        else if (readString.startsWith("#6")) {
          lowest_time = readString.substring(2);
          Serial.println(lowest_time);
        }
        
        else if (readString.startsWith("#7")) {
          highest_temp = readString.substring(2);
          Serial.println(highest_temp);
        }
        
        else if (readString.startsWith("#8")) {
          highest_time = readString.substring(2);
          Serial.println(highest_time);
        }
        readString=""; //clears variable for new input
      }
    }
    
    else if (c =='@') {
      if (readString.length() >0) {
        if (readString == "blue") {
          setColor(0, 0, 255);  // blue
        }
        else if (readString == "green") {
          setColor(0, 255, 0);  // green
        }
        else if (readString == "red") {
          setColor(255, 0, 0);  // red
        }
        else if (readString == "yellow") {
          setColor(255, 255, 0);  // yellow
        }
        else if (readString == "purple") {
          setColor(175, 0, 175);  // purple
        }
        else if (readString == "aqua") {
          setColor(0, 255, 255);  // aqua
      
    }
    }
    readString=""; //clears variable for new input
    }
    
    else {     
      readString += c; //makes the string readString
    }
  }
  // -------------- Debound code start --------------
  int reading = digitalRead(button);
  buttonState1 = digitalRead(button);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;
      
        if (buttonState1 != lastButtonState1) {
          
          if (buttonState1 == LOW) {
            buttonPushCounter1++;
            
            if (buttonPushCounter1 == 9) {
              buttonPushCounter1 = 1;}
              lcd.clear();
            //Serial.println("LCD clear");
            //Serial.println("on");
            //Serial.print("number of button pushes:  ");
            //Serial.println(buttonPushCounter1, DEC);
          }
          else {
            //Serial.println("off"); 
          }
        }
          lastButtonState1 = buttonState1;
    }
  }
  lastButtonState = reading;
  // -------------- Debound code end --------------
  
    if (buttonPushCounter1 == 1) {
      lcd.print("--Current temp--");
      lcd.setCursor(0,1);
      lcd.print(temp_c);
      //lcd.print(char(0));
      //lcd.write(165);
      lcd.print("C");
    }
    
    if (buttonPushCounter1 == 2) {
      lcd.print("---Feels like---");
      lcd.setCursor(0,1);
      lcd.print(feels);
      //lcd.print(char(0));
      lcd.print("C");
    }

    if (buttonPushCounter1 == 3) {
      lcd.print("-----Weather----");
      lcd.setCursor(0,1);
      lcd.print(weather);
    }

    if (buttonPushCounter1 == 4) {
      lcd.print("---Wind speed---");
      lcd.setCursor(0,1);
      lcd.print(wind);
    }

    if (buttonPushCounter1 == 5) {
      lcd.print("--Lowest temp---");
      lcd.setCursor(0,1);
      lcd.print(lowest_time);
      lcd.print(" ");
      lcd.print(lowest);
      //lcd.print(char(0));
      lcd.print("C");
    }
    
    if (buttonPushCounter1 == 6) {
      lcd.print("--Highest temp--");
      lcd.setCursor(0,1);
      lcd.print(highest_time);
      lcd.print(" ");
      lcd.print(highest_temp);
      //lcd.print(char(0));
      lcd.print("C");
    }
    

  if (buttonPushCounter1 == 7) {
          
    char buffer_minutes[3];
      char buffer_seconds[3];
      //lcd.clear();
      lcd.setCursor(0,0);
      readTime();
      lcd.print(days[weekday-1]);
      lcd.setCursor(0,1);
      lcd.print(monthday);
      lcd.print("/");
      lcd.print(month);
      lcd.print("/");
      lcd.print(year);
      lcd.print(" ");
      lcd.print(hour);
      lcd.print(":");
      sprintf(buffer_minutes, "%02d", minute);
      lcd.print(buffer_minutes);
      lcd.print(":");
      sprintf(buffer_seconds, "%02d", second);
      lcd.print(buffer_seconds);
  }
  
  
    if (buttonPushCounter1 == 8) {
      if (millis() >= waitUntilcycle1) {
      lcd.clear();
      lcd.print("--Current temp--");
      lcd.setCursor(0,1);
      lcd.print(temp_c);
      //lcd.print(char(0));
      lcd.print("C");
      waitUntilcycle1 += 35000;  
      }
    if (millis() >= waitUntilcycle2) {
      lcd.clear();
      lcd.print("---Feels like---");
      lcd.setCursor(0,1);
      lcd.print(feels);
      //lcd.print(char(0));
      lcd.print("C");
      waitUntilcycle2 += 35000;
     }
    if (millis() >= waitUntilcycle3) {
      lcd.clear();
      lcd.print("-----Weather----");
      lcd.setCursor(0,1);
      lcd.print(weather);
      waitUntilcycle3 += 35000;
     }
    if (millis() >= waitUntilcycle4) {
      lcd.clear();
      lcd.print("---Wind speed---");
      lcd.setCursor(0,1);
      lcd.print(wind);
      waitUntilcycle4 += 35000;
    }
    if (millis() >= waitUntilcycle5) {
      lcd.clear();
      lcd.print("--Lowest temp---");
      lcd.setCursor(0,1);
      lcd.print(lowest_time);
      lcd.print(" ");
      lcd.print(lowest);
      //lcd.print(char(0));
      lcd.print("C");
      waitUntilcycle5 += 35000;
    }
    if (millis() >= waitUntilcycle6) {
      lcd.clear();
      lcd.print("--Highest temp--");
      lcd.setCursor(0,1);
      lcd.print(highest_time);
      lcd.print(" ");
      lcd.print(highest_temp);
      //lcd.print(char(0));
      lcd.print("C");
      waitUntilcycle6 += 35000;
    }
    if (millis() >= waitUntilcycle7) {
      char buffer_minutes[3];
      char buffer_seconds[3];
      lcd.clear();
      lcd.setCursor(0,0);
      readTime();
      lcd.print(days[weekday-1]);
      lcd.setCursor(0,1);
      lcd.print(monthday);
      lcd.print("/");
      lcd.print(month);
      lcd.print("/");
      lcd.print(year);
      lcd.print(" ");
      lcd.print(hour);
      lcd.print(":");
      sprintf(buffer_minutes, "%02d", minute);
      lcd.print(buffer_minutes);
      waitUntilcycle7 += 35000;
  }
  }
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

/*void printTime() {
  char buffer[3];
  const char* AMPM = 0;
  readTime();
  Serial.print(days[weekday-1]);
  Serial.print(" ");
  Serial.print(months[month-1]);
  Serial.print(" ");
  Serial.print(monthday);
  Serial.print(", 20");
  Serial.print(year);
  Serial.print(" ");
  if (hour > 12) {
    hour -= 12;
    AMPM = " PM";
  }
  else AMPM = " AM";
  Serial.print(hour);
  Serial.print(":");
  sprintf(buffer, "%02d", minute);
  Serial.print(buffer);
  Serial.println(AMPM);
}*/


void readTime() {
  Wire.beginTransmission(DS1307);
  Wire.write(byte(0));
  Wire.endTransmission();
  Wire.requestFrom(DS1307, 7);
  second = bcdToDec(Wire.read());
  minute = bcdToDec(Wire.read());
  hour = bcdToDec(Wire.read());
  weekday = bcdToDec(Wire.read());
  monthday = bcdToDec(Wire.read());
  month = bcdToDec(Wire.read());
  year = bcdToDec(Wire.read());
}

byte decToBcd(byte val) {
  return ((val/10*16) + (val%10));
}
byte bcdToDec(byte val) {
  return ((val/16*10) + (val%16));
}


