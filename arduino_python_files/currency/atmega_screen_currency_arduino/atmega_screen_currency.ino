/*
Copyright (C) 2014  Dale Beever

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see http://www.gnu.org/licenses/.
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String readString; // readstring is for collecting the serial buffer code
String USD;  // string that stores USD
String EUR;  // string that stores EUR
String bitcoin;  // string that stores bitcoin

uint8_t pound[8] = {0x7,0x8,0x8,0x1e,0x8,0x8,0x1f}; // Custom char british pounds
uint8_t euro[8] = {0x3,0x4,0x8,0x1f,0x8,0x1f,0x4,0x3}; // Custom char euro

//Green Button stuff
const int button_green = 5;
int buttonPushCounter1_green = 4;    // counts the button pushes
int buttonState1_green = 0;    // tracks the button state
int lastButtonState1_green = 0;    // last state of the button

// Debounce stuff Green Button
int buttonState_green;             // the current reading from the input pin
int lastButtonState_green = LOW;
long lastDebounceTime_green = 0;  // the last time the output pin was toggled
long debounceDelay_green = 50;    // the debounce time; increase if the output flickers

// Button red top stuff
const int button_top_red = 7; // Button set to pin 7
long time = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers
int state = 1;      // the current state of the output pin
int reading_red_top;           // the current reading from the input pin
int previous = LOW;    // the previous reading from the input pin

// Timing for the display cycle
long waitUntilcycle1 = 0;
long waitUntilcycle2 = 5000;
long waitUntilcycle3 = 10000;

void setup() {
  pinMode(button_green, INPUT); // Set the button as input
  digitalWrite(button_green, HIGH); // initiate the internal pull up resistor
  pinMode(button_top_red, INPUT); // Set the button as input
  digitalWrite(button_top_red, HIGH); // initiate the internal pull up resistor
  
  Serial.begin(9600); // Begin serial at 9600 baud
  Serial.println("Waiting"); // Print to serial 
  
  Wire.begin(); // Start the wire liberty
  lcd.backlight(); // Turn on the lcd backlight
  lcd.init(); // Start up the lcd
  lcd.begin(16, 2); // Set up the lcd to have 16 char on 2 lines
  lcd.createChar(0, pound);  //  Custom Char british pound
  lcd.createChar(1, euro); // Custom char euro
  
  // Splash screen
  lcd.setCursor(1,0); // Set lcd cursor to start of first line
  lcd.print("#atmega_screen");
  lcd.setCursor(1,1); // Set lcd cursor to 2nd char on 2nd line
  lcd.print("Currency rates");
  delay(1000);
}

void loop() {
  lcd.setCursor(0,0);
  if (Serial.available())  {
    char c = Serial.read();  //gets one byte from serial buffer
    
    if (c == ';') { // If the readString contains ; then it passes on to the next if statments
      if (readString.length() >0) { // Also it has to have a lenght of more then 0
        
        if (readString.startsWith("#1#")) { // If readString starts with #1# then it passes it to USD
          USD = readString.substring(3);
          Serial.println(USD);
        }
        
        else if (readString.startsWith("#2#")) { // If readString starts with #2# then it passes it to EUR
          EUR = readString.substring(3);
          Serial.println(EUR);
        }
        
        else if (readString.startsWith("#3#")) { // If readString starts with #3# then it passes it to bitcoin
          bitcoin = readString.substring(3);
          Serial.println(bitcoin);
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
  buttonState1_green = digitalRead(button_green); // Load button stat into buttonState1

  if (reading_green != lastButtonState_green) { // Check if reading doesnt equal lastButtonState
    lastDebounceTime_green = millis();
  } 
  
  if ((millis() - lastDebounceTime_green) > debounceDelay_green) {

    if (reading_green != buttonState_green) {
      buttonState_green = reading_green;
      
        if (buttonState1_green != lastButtonState1_green) { // Check buttonState1 doesnt equal lastButtonState1
          
          if (buttonState1_green == LOW) { // Check if buttonStata1 == LOW
            buttonPushCounter1_green++; // Increment buttonPushCounter1
            
            if (buttonPushCounter1_green == 5) { // Check buttonPushCounter1 is == 5
              buttonPushCounter1_green = 1;} // If buttonPushCounter == 5 then set buttonPushCounter1 to 1
              lcd.clear(); // Clear the lcd
          }
          else {
            //Serial.println("off"); 
          }
        }
          lastButtonState1_green = buttonState1_green;
    }
  }
  lastButtonState_green = reading_green;
  // -------------- Debound code end Green --------------
  
    // -------------- Debound code button top red start code--------------

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  reading_red_top = digitalRead(button_top_red);

  if (reading_red_top == HIGH && previous == LOW && millis() - time > debounce) { 
    if (state == 1) {
      state = 0;
      lcd.backlight(); // Turn backlight on
    }
    else {
      state = 1;
      lcd.noBacklight(); // Turn off backlight
    }

    time = millis();    
  }
  //Serial.println(state);
  previous = reading_red_top;
   
  // -------------- Debound code button top red end code --------------

    if (buttonPushCounter1_green == 1) {
      lcd.setCursor(0,0); // Set cursor to start of first line
      lcd.print("------USD-------");
      lcd.setCursor(0,1); // Set cursor to start of second line
      lcd.write(36); // Print US dollar sign to LCD
      lcd.print("1 = ");
      lcd.print((char)0); // Print custom pound sign to LCD      
      lcd.print(USD);
    }
    
    if (buttonPushCounter1_green == 2) {
      lcd.setCursor(0,0); // Set cursor to start of first line
      lcd.print("------EUR-------");
      lcd.setCursor(0,1); // Set cursor to start of second line
      lcd.print((char)1); // Print Euro sign to LCD
      lcd.print("1 = ");
      lcd.print((char)0); // Print custom pound sign to LCD
      lcd.print(EUR);
    }

    if (buttonPushCounter1_green == 3) {
      lcd.setCursor(0,0); // Set cursor to start of first line
      lcd.print("----Bitcoin-----");
      lcd.setCursor(0,1); // Set cursor to start of second line
      lcd.print((char)0); // Print custom pound sign to LCD
      lcd.print(bitcoin);
    }
    
    if (buttonPushCounter1_green == 4) {
      if (millis() >= waitUntilcycle1) {
        lcd.clear(); // Clear LCD
        lcd.setCursor(0,0); // Set cursor to start of first line
        lcd.print("------USD-------");
        lcd.setCursor(0,1); // Set cursor to start of second line
        lcd.write(36); // Print US dollar sign to LCD
        lcd.print("1 = ");
        lcd.print((char)0); // Print custom pound sign to LCD      
        lcd.print(USD);
        waitUntilcycle1 += 15000;  
      }
    if (millis() >= waitUntilcycle2) {
      lcd.clear(); // Clear LCD
      lcd.setCursor(0,0); // Set cursor to start of first line
      lcd.print("------EUR-------");
      lcd.setCursor(0,1); // Set cursor to start of second line
      lcd.print((char)1); // Print Euro sign to LCD
      lcd.print("1 = ");
      lcd.print((char)0); // Print custom pound sign to LCD
      lcd.print(EUR);
      waitUntilcycle2 += 15000;
     }
    if (millis() >= waitUntilcycle3) {
      lcd.clear(); // Clear LCD
      lcd.setCursor(0,0); // Set cursor to start of first line
      lcd.print("----Bitcoin-----");
      lcd.setCursor(0,1); // Set cursor to start of second line
      lcd.print((char)0); // Print custom pound sign to LCD
      lcd.print(bitcoin);
      waitUntilcycle3 += 15000;
     }
    }
}
