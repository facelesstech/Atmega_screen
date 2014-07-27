#include <rtttl.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String readString; // readstring is for collecting the serial buffer code
String temp_c;  // string that stores temp
String feels;  // string that stores feels like temp
String weather;  // string that stores weather forcast
String lowest_temp;  // string that stores lowest temp
String wind;  // string that stores wind speed
String lowest_time;  // Time last lowest
String highest_temp;  // string that stores highest temp
String highest_time;   // string that stores highest temp time

uint8_t degree[8] = {0x8,0xf4,0x8,0x43,0x4,0x4,0x43,0x0}; // Custom char degrees c
uint8_t left[8] = {0x0,0x0,0x0,0x3,0x7,0xf,0xf,0x1f};
uint8_t centre[8] = {0x0,0x0,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
uint8_t right[8] = {0x0,0x0,0x0,0xf8,0x1c,0x1e,0x1e,0x1f};

//RTTTL player stuff
int threshold = 30;  // Sound sensor threshold - lower for more sensative higher for less
int volume;  // Readings from sound sensor
int pinSpeaker = 8;  // Speaker pin 1
const int octave = 0;  // The octave set for the player
const char button_green_tone[] PROGMEM ="button:16e";
const char button_red_backlight_tone[] PROGMEM ="button:16e";
Rtttl player;  // Song player

// Button green stuff
const int button_green = 5; // Set button to pin 5
int buttonPushCounter1 = 8;    // counts the button pushes
int buttonState1 = 0;    // tracks the button state
int lastButtonState1 = 0;    // last state of the button

// Debounce green button
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;  // last button state set to low
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Button red top stuff
const int button_top_red = 7; // Button set to pin 7
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
int state = 1;      // the current state of the output pin
int reading_red_top;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// Button red bottom stuff
const int button_bottom_red = 6; // Button set to pin 6
long time_bottom_red = 0;         // the last time the output pin was toggled
long debounce_bottom_red = 200;   // the debounce time, increase if the output flickers
int LED_state = 1;      // the current state of the output pin
int reading_bottom_red;           // the current reading from the input pin
int previous_bottom_red = LOW;    // the previous reading from the input pin

String led_colour;

// Timing for the display cycle times
long waitUntilcycle1 = 0;
long waitUntilcycle2 = 5000;
long waitUntilcycle3 = 10000;
long waitUntilcycle4 = 15000;
long waitUntilcycle5 = 20000;
long waitUntilcycle6 = 25000;
long waitUntilcycle7 = 30000;

// RGB LED
int redPin = 12; // Red led set to pin 12
int greenPin = 11; // Green led set to pin 11
int bluePin = 10; // Blue led set to pin 10
#define COMMON_ANODE

// RTC 
const int DS1307 = 0x68; // Address of DS1307 see data sheets
const char* days[] =
{"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"}; // converts the numbers from the days of week to names

// Initializes all values: 
byte second = 0; // Setting up bytes for seconds
byte minute = 0; // Setting up bytes for minutes
byte hour = 0; // Setting up bytes for hours
byte weekday = 0; // Setting up bytes for weekday
byte monthday = 0; // Setting up bytes for monthday
byte month = 0; // Setting up bytes for month
byte year = 0;  // Setting up bytes for year


void setup() {
  player.begin(pinSpeaker);  // Starts the player
  pinMode(pinSpeaker, OUTPUT);  // Speaker set as output

  Wire.begin(); // Start the wire liberty
  lcd.backlight(); // Turn on the lcd backlight
  lcd.init(); // Start up the lcd
  lcd.begin(16, 2); // Set up the lcd to have 16 char on 2 lines
  lcd.createChar(0, degree);
  lcd.createChar(1, left);
  lcd.createChar(2, centre);
  lcd.createChar(3, right);
  
  // Splash screen
  lcd.setCursor(1,0); // Set lcd cursor to start of first line
  lcd.print("#atmega_screen");
  lcd.setCursor(1,1);
  lcd.print("Weather screen");
  delay(1000);
  /*lcd.setCursor(0,0);
  lcd.print("Weather screen");
  lcd.setCursor(0,1);
  lcd.print((char)1);
  lcd.print((char)2);
  lcd.print((char)3);
  lcd.print("V0.1");
  delay(1000);*/
  
  pinMode(button_green, INPUT); // Set the button as input
  digitalWrite(button_green, HIGH); // initiate the internal pull up resistor
  pinMode(button_top_red, INPUT); // Set the button as input
  digitalWrite(button_top_red, HIGH); // initiate the internal pull up resistor
  pinMode(button_bottom_red, INPUT); // Set the button as input
  digitalWrite(button_bottom_red, HIGH); // initiate the internal pull up resistor
  
  pinMode(redPin, OUTPUT); // Set red led pin to output
  pinMode(greenPin, OUTPUT); // Set green led pin to output
  pinMode(bluePin, OUTPUT); // Set blue led pin to output
  
  Serial.begin(9600); // Begin serial at 9600 baud
  Serial.println("Waiting"); // Print to serial 
}

void loop() {
  
  if (LED_state == 1) {
    setColor(0, 0, 0);
  }
      
  else {
      
    if (led_colour == "aqua") {
      setColor(0, 255, 255); // Sets the RGB colour to aqua
    }
    
    else if (led_colour == "blue") {
      setColor(0, 0, 255); // Sets the RGB colour to blue
    }
          
    else if (led_colour == "yellow") {
      setColor(255, 255, 0); // Sets the RGB colour to yellow
    }
    
    else if (led_colour == "orange") {
      setColor(255, 127, 0); // Sets the RGE colour to orange
    }
    
    else if (led_colour == "red") {
      setColor(255, 0, 0);  // Sets the RGB colour to red
    }
  }
  
  lcd.setCursor(0,0); // Set lcd cursor to the start of the first row
  if (Serial.available())  { // Check to see if serial is available
    char c = Serial.read();  //gets one byte from serial buffer
    
    if (c == ';') { // Checks to see if c contains ;
      if (readString.length() >0) { // Checks that the length of readString is over 0
        if (readString.startsWith("#1")) { // Checks readString if it starts with #1
          temp_c = readString.substring(2); // Adds the readString less the first 2 characters to temp_c
          Serial.println(temp_c); // Prints temp_c to serial
        }
        
        else if (readString.startsWith("#2")) { // Checks readString if it starts with #2
          feels = readString.substring(2); // Adds the readString less the first 2 characters to feels
          Serial.println(feels); // Prints feels to serial
        }
        
        else if (readString.startsWith("#3")) { // Checks readString if it starts with #3
          weather = readString.substring(2); // Adds the readString less the first 2 characters to weather
          Serial.println(weather); // Prints weather to serial
        }
        
        else if (readString.startsWith("#4")) { // Checks readString if it starts with #4
          lowest_temp = readString.substring(2); // Adds the readString less the first 2 characters to lowest_temp
          Serial.println(lowest_temp); // Prints lowest_temp to serial
        }
        
        else if (readString.startsWith("#5")) { // Checks readString if it starts with #5
          wind = readString.substring(2); // Adds the readString less the first 2 characters to wind
          Serial.println(wind); // Prints wind to serial
        }
        
        else if (readString.startsWith("#6")) { // Checks readString if it starts with #6
          lowest_time = readString.substring(2); // Adds the readString less the first 2 characters to lowest_time
          Serial.println(lowest_time); // Prints lowest_time to serial
        }
        
        else if (readString.startsWith("#7")) { // Checks readString if it starts with #7
          highest_temp = readString.substring(2); // Adds the readString less the first 2 characters to highest_temp
          Serial.println(highest_temp); // Prints highest_temp to serial
        }
        
        else if (readString.startsWith("#8")) { // Checks readString if it starts with #8
          highest_time = readString.substring(2); // Adds the readString less the first 2 characters to highest_time
          Serial.println(highest_time); // Prints highest_time to serial
        }
        
        else if (readString.startsWith("#blue#")) { // Check if readString contains blue
          led_colour = readString.substring(6);
          //setColor(0, 0, 255); // Sets the RGB colour to blue
        }
        
        else if (readString.startsWith("#green#")) { // Check if readString contains green
          led_colour = readString.substring(7);
          //setColor(0, 255, 0); // Sets the RGB colour to green
        }
        
        else if (readString.startsWith("#red#")) { // Check if readString contains red
          led_colour = readString.substring(5);
          //setColor(255, 0, 0);  // Sets the RGB colour to red
        }
        
        else if (readString.startsWith("#yellow#")) { // Check if readString contains yellow
          led_colour = readString.substring(8);
          //setColor(255, 255, 0); // Sets the RGB colour to yellow
        }
        
        else if (readString.startsWith("#orange#")) { // Check if readString contains yellow
          led_colour = readString.substring(8);
          //setColor(255, 127, 0); // Sets the RGB colour to orange
        }
        
        else if (readString.startsWith("#purple#")) { // Check if readString contains purple
          led_colour = readString.substring(8);
          //setColor(175, 0, 175); // Sets the RGB colour to purple
        }
        
        else if (readString.startsWith("#aqua#")) { // Check if readString contains aqua
          led_colour = readString.substring(6);
          //setColor(0, 255, 255); // Sets the RGB colour to aqua
        }
        
        readString=""; //clears variable for new input
      }
    }
    else {     
      readString += c; //makes the string readString
    }
    
  }
  // -------------- Debound code start Green --------------
  int reading_green = digitalRead(button_green); // Load button stat to int reading
  buttonState1 = digitalRead(button_green); // Load button stat into buttonState1

  if (reading_green != lastButtonState) { // Check if reading doesnt equal lastButtonState
    lastDebounceTime = millis(); 
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading_green != buttonState) {
      buttonState = reading_green; 
      
        if (buttonState1 != lastButtonState1) { // Check buttonState1 doesnt equal lastButtonState1
          
          if (buttonState1 == LOW) { // Check if buttonStata1 == LOW
            buttonPushCounter1++; // Increment buttonPushCounter1
            
            if (buttonPushCounter1 == 9) { // Check buttonPushCounter1 is == 9
              buttonPushCounter1 = 1;} // If buttonPushCounter == 9 then set buttonPushCounter1 to 1
              lcd.clear(); // Clear the lcd
              playSong(button_green_tone); // Play button sound
          }
          else {
            //Serial.println("off"); 
          }
        }
          lastButtonState1 = buttonState1; 
    }
  }
  lastButtonState = reading_green;
  // -------------- Debound code end Green --------------
  
  // -------------- Debound code button top red start code--------------

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  reading_red_top = digitalRead(button_top_red);

  if (reading_red_top == HIGH && previous == LOW && millis() - time > debounce) { 
    if (state == 1) {
      state = 0;
      lcd.backlight();
    }
    else {
      state = 1;
      lcd.noBacklight();
    }

    time = millis();    
  }
  //Serial.println(state);
  previous = reading_red_top;
   
  // -------------- Debound code button top red end code --------------
  
// -------------- Debound code button bottom red start code--------------

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  reading_bottom_red = digitalRead(button_bottom_red);

  if (reading_bottom_red == HIGH && previous_bottom_red == LOW && millis() - time_bottom_red > debounce_bottom_red) {
    if (LED_state == 1) {
      LED_state = 0;
    }
    else {
      LED_state = 1;
    }

    time_bottom_red = millis();    
  }
    Serial.println(LED_state);
  previous_bottom_red = reading_bottom_red;
   
  // -------------- Debound code button bottom red end code --------------
  
    if (buttonPushCounter1 == 1) {
      //lcd.print((char)1);
      //lcd.print((char)2);
      lcd.print("--Current temp--");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(temp_c); // Lcd print temp_c string
      lcd.print((char)0);
    }
    
    if (buttonPushCounter1 == 2) {
      lcd.print("---Feels like---");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(feels); // Lcd print feels string
      lcd.print((char)0);
    }

    if (buttonPushCounter1 == 3) {
      lcd.print("-----Weather----");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(weather); // Lcd print weather String
    }

    if (buttonPushCounter1 == 4) {
      lcd.print("---Wind speed---");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(wind); // Lcd print wind string
    }

    if (buttonPushCounter1 == 5) {
      lcd.print("--Lowest temp---");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(lowest_time); // Lcd print lowest_time string
      lcd.print(" ");
      lcd.print(lowest_temp); // Lcd print lowest_temp string
      lcd.print((char)0);
    }
    
    if (buttonPushCounter1 == 6) {
      lcd.print("--Highest temp--");
      lcd.setCursor(0,1); // Set cursor to start of the second screen
      lcd.print(highest_time); // Lcd print highest_time string
      lcd.print(" ");
      lcd.print(highest_temp); // Lcd print highest_temp string
      lcd.print((char)0);
    }
    

  if (buttonPushCounter1 == 7) {
          
    char buffer_minutes[3]; // Creates a char buffer of 3 characters for minutes
      char buffer_seconds[3]; // Creates a char buffer of 3 characters for seconds
      lcd.setCursor(0,0); // Set lcd cursor to start of first line
      readTime(); // Runs readTime to get RTC data
      lcd.print(days[weekday-1]);
      lcd.setCursor(0,1); // Set lcd cursor to start of second line
      lcd.print(monthday);
      lcd.print("/");
      lcd.print(month);
      lcd.print("/");
      lcd.print(year);
      lcd.print(" ");
      lcd.print(hour);
      lcd.print(":");
      sprintf(buffer_minutes, "%02d", minute); // Floats the minutes to only 2 decimal points stores them on buffer
      lcd.print(buffer_minutes);
      lcd.print(":");
      sprintf(buffer_seconds, "%02d", second); // Floats the seconds to only 2 decimal points stores them on buffer
      lcd.print(buffer_seconds);
  }
  
  
    if (buttonPushCounter1 == 8) {
      if (millis() >= waitUntilcycle1) {
      lcd.clear();
      lcd.print("--Current temp--");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(temp_c); // Lcd print temp_c string
      lcd.print((char)0);
      waitUntilcycle1 += 35000; // Addes 35000 to waitUntilcycle1
      }
    if (millis() >= waitUntilcycle2) {
      lcd.clear();
      lcd.print("---Feels like---"); // Lcd print feels string
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(feels); // Lcd print feels String
      lcd.print((char)0);
      waitUntilcycle2 += 35000;
     }
    if (millis() >= waitUntilcycle3) {
      lcd.clear();
      lcd.print("-----Weather----");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(weather); // Lcd print weather String
      waitUntilcycle3 += 35000;
     }
    if (millis() >= waitUntilcycle4) {
      lcd.clear();
      lcd.print("---Wind speed---");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(wind); // Lcd print wind string
      waitUntilcycle4 += 35000;
    }
    if (millis() >= waitUntilcycle5) {
      lcd.clear();
      lcd.print("--Lowest temp---");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(lowest_time); // Lcd print lowest_time string
      lcd.print(" ");
      lcd.print(lowest_temp); // Lcd print lowest_temp string
      lcd.print((char)0);
      waitUntilcycle5 += 35000;
    }
    if (millis() >= waitUntilcycle6) {
      lcd.clear();
      lcd.print("--Highest temp--");
      lcd.setCursor(0,1); // Ser cursor to start of second line
      lcd.print(highest_time); // Lcd print highest_time string
      lcd.print(" ");
      lcd.print(highest_temp); // Lcd print highest_temp string
      lcd.print((char)0);
      waitUntilcycle6 += 35000;
    }
    if (millis() >= waitUntilcycle7) {
      char buffer_minutes[3]; // Creates a char buffer of 3 characters for minutes
      char buffer_seconds[3]; // Creates a char buffer of 3 characters for seconds
      lcd.clear();
      lcd.setCursor(0,0); // Set lcd cursor to start of first line
      readTime(); // Runs readTime to get RTC data
      lcd.print(days[weekday-1]);
      lcd.setCursor(0,1); // Set lcd cursor to start of second line
      lcd.print(monthday);
      lcd.print("/");
      lcd.print(month);
      lcd.print("/");
      lcd.print(year);
      lcd.print(" ");
      lcd.print(hour);
      lcd.print(":");
      sprintf(buffer_minutes, "%02d", minute); // Floats the minutes to only 2 decimal points stores them on buffer
      lcd.print(buffer_minutes);
      waitUntilcycle7 += 35000;
  }
  }
}

// Set RGB colours
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

// RTC data
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

void playSong(const char * track)  // Takes the song_# pasted from the if statements below
{
  player.play_P(track, octave);  // Passes the song_# and octave to the player
}

