#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

String readString; // readstring is for collecting the serial buffer code
String GOOG;  // string that stores GOOG
String GOOG_LED; // string that stores LED status
String AAPL;  // string that stores AAPL
String AAPL_LED; // string that stores LED status
String TWTR;  // string that stores TWTR
String TWTR_LED; // string that stores LED status
String FB;  // string that stores FB
String FB_LED; // string that stores LED status


// Custom Pound sign for LCD
byte pound[8] = {
  B00111,
  B01000,
  B01000,
  B11110,
  B01000,
  B01000,
  B11111,
};


// Button stuff
const int button = 3; // Button set to pin 3
int buttonPushCounter1 = 5;    // counts the button pushes
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

// Sets the led pins
int redPin = 12;
int greenPin = 11;
int bluePin = 10;
// Comment out if not a COMMON_ANODE
#define COMMON_ANODE

void setup() {
  lcd.backlight();
  lcd.init();
  pinMode(button, INPUT);
  digitalWrite(button, HIGH);
  //lcd.createChar(0, pound);  //  Custom Char
  Serial.begin(9600); // Start serial
  Serial.println("Waiting"); // Print serial
  lcd.begin(16, 2); // Start LCD screen off  
}

void loop() {
  lcd.setCursor(0,0); // Set cursor to 0,0
  if (Serial.available())  { // Check to see if serial is available
    char c = Serial.read();  //gets one byte from serial buffer
    
    if (c == ';') {
      if (readString.length() >0) {
        if (readString.startsWith("#1")) { // Checks the first part of the string
          GOOG = readString.substring(2); // Save to string
          Serial.println(GOOG); // Prints the String to serial
      }
      
        else if (readString.startsWith("#2")) { // Checks the first part of the string
          GOOG_LED = readString.substring(2); // Save to string
          Serial.println(GOOG_LED); // Prints the String to serial
      }
        
        else if (readString.startsWith("#3")) { // Checks the first part of the string
          AAPL = readString.substring(2); // Save to string
          Serial.println(AAPL); // Prints the String to serial
      }
      
        else if (readString.startsWith("#4")) { // Checks the first part of the string
          AAPL_LED = readString.substring(2); // Save to string
          Serial.println(AAPL_LED); // Prints the String to serial
      }
        
        else if (readString.startsWith("#5")) { // Checks the first part of the string
          TWTR = readString.substring(2); // Save to string
          Serial.println(TWTR); // Prints the String to serial
      }
    
        else if (readString.startsWith("#6")) { // Checks the first part of the string
          TWTR_LED = readString.substring(2); // Save to string
          Serial.println(TWTR_LED); // Prints the String to serial
      }
    
        else if (readString.startsWith("#7")) { // Checks the first part of the string
          FB = readString.substring(2); // Save to string
          Serial.println(FB); // Prints the String to serial
      }
      
        else if (readString.startsWith("#8")) { // Checks the first part of the string
          FB_LED = readString.substring(2); // Save to string
          Serial.println(FB_LED); // Prints the String to serial
      }
    
        readString=""; //clears variable for new input
      }
    }

    else {     
      readString += c; //makes the string readString
    }
  }
  // -------------- Debound code --------------
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
            
            if (buttonPushCounter1 == 6) {
              buttonPushCounter1 = 1;}
            lcd.clear();
        
          }
          else {
          }
        }
          lastButtonState1 = buttonState1;
    }
  }
  lastButtonState = reading;
  
    if (buttonPushCounter1 == 1) { 
      lcd.print("GOOG stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.write(237); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(GOOG); // Print LCD stock price
      
      if (GOOG_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (GOOG_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }

    }
    
    if (buttonPushCounter1 == 2) {
      lcd.print("AAPL stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(AAPL); // Print LCD stock price
      
      if (AAPL_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (AAPL_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
    }

    if (buttonPushCounter1 == 3) {
      lcd.print("TWTR stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(TWTR); // Print LCD stock price
      
      if (TWTR_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (TWTR_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
    }
    
    if (buttonPushCounter1 == 4) {
      lcd.print("FB stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(FB); // Print LCD stock price
      
      if (FB_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (FB_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
      
    }
    
    if (buttonPushCounter1 == 5) {
      if (millis() >= waitUntilcycle1) {
      lcd.clear();
      lcd.print("GOOG stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(GOOG); // Print LCD stock price
      
      if (GOOG_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (GOOG_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
      
      waitUntilcycle1 += 20000;  
      }
    if (millis() >= waitUntilcycle2) {
      lcd.clear();
      lcd.print("AAPL stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(AAPL); // Print LCD stock price
      
      if (AAPL_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (AAPL_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
      
      waitUntilcycle2 += 20000;
     }
   if (millis() >= waitUntilcycle3) {
      lcd.clear();
      lcd.print("TWTR stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(TWTR); // Print LCD stock price
      
      if (TWTR_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (TWTR_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
      
      waitUntilcycle3 += 20000;
     }
   if (millis() >= waitUntilcycle4) {
      lcd.clear();
      lcd.print("FB stock price"); // Print LCD stock price
      lcd.setCursor(0,1); // Change cursor position
      //lcd.print((char)0); // Print custom pound sign to LCD
      lcd.setCursor(1,1); // Change cursor position
      lcd.print(FB); // Print LCD stock price
      
      if (FB_LED == "1") {
        setColor(0, 255, 0);  // green
      }
      else if (FB_LED == "2") {
        setColor(255, 0, 0);  // red
      }
      else {
        setColor(0, 0, 0);  // off
      }
      
      waitUntilcycle4 += 20000;
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
