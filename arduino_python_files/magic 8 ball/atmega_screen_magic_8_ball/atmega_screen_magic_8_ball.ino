#include <rtttl.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//RTTTL player stuff
int threshold = 30;  // Sound sensor threshold - lower for more sensative higher for less
int volume;  // Readings from sound sensor
int pinSpeaker = 8;  // Speaker pin 1
const int octave = 0;  // The octave set for the player
const char song_1[] PROGMEM =
"button:16e"; // Used the rtttl player to make the button beep sound
Rtttl player;  // Song player

int magic_8_random = 21; // Set int to 21 to no set off if statments below

// Button stuff
const int button_green = 5; // green button on pin 5
int buttonState1 = 0;    // Tracks the button state
int lastButtonState1 = 0; // Tracks last button state

// Debounce stuff
int buttonState;             // The current reading from the input pin
int lastButtonState = HIGH;  // last button state set to low
long lastDebounceTime = 0;  // The last time the output pin was toggled
long debounceDelay = 50;    // The debounce time; increase if the output flickers

void setup() {
  player.begin(pinSpeaker);  // Starts the player
  pinMode(pinSpeaker, OUTPUT);  // Speaker set as output
  
  lcd.backlight(); // Turn on backlight
  lcd.init(); // Start the lcd screen
  lcd.begin(16, 2);  // Set up the lcd to have 16 char on 2 lines
  
  lcd.setCursor(0,0); // Sets cursor to the third char on the first row
  lcd.print("Magic 8 ball v1"); 
  lcd.setCursor(1,1); // Sets cursor to the second char on the second row
  lcd.print("<- Press HERE");
 
  pinMode(button_green, INPUT); // Set green button to input
  digitalWrite(button_green, HIGH); // Turn on the internal pull up resistor
  
  Serial.begin(9600); // Begin serial at 9600 baud
  Serial.println("Starting"); // Serial print
}
  
void loop() {
    // -------------- Debound code green button start --------------
  int reading = digitalRead(button_green); // set button stat to int reading
  buttonState1 = digitalRead(button_green); // set button stat into buttonState1

  if (reading != lastButtonState) { // Check if reading doesnt equal lastButtonState
    lastDebounceTime = millis(); // Set lastDebouncetime to the current millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) { // Check if reading doesn't equal buttonState
      buttonState = reading; // Set buttonState to reading
      
        if (buttonState1 != lastButtonState1) { // Check buttonState1 doesnt equal lastButtonState1
          
          if (buttonState1 == LOW) { // Check if buttonStata1 == LOW
          magic_8_random = random(0,20); // Pick random number between 0 and 20
          lcd.clear(); // Clear LCD
          playSong(song_1); // Play button sound
          
          }
        
      else {
            Serial.println("off"); 
          }
          }
      lastButtonState1 = buttonState1; // Set lastButtonState1 to buttonState1
    }
  }

  lastButtonState = reading; // Set lastButtonState to reading
  // -------------- Debound code green button end --------------
    
        if (magic_8_random == 0)  {
          Serial.println("It is certain");
          lcd.setCursor(1,0); // Move cursor to 2nd char on the first line
          lcd.print("It is certain");
        }
        
        if (magic_8_random == 1)  {
          Serial.println("It is decidedly so");
          lcd.setCursor(0,0); // Move cursor to the start of the first line
          lcd.print("It is decidedly");
          lcd.setCursor(7,1); // Move cursor to 7th char on the second line
          lcd.print("so");
        }
        
        if (magic_8_random == 2)  {
          Serial.println("Without a doubt");
          lcd.setCursor(0,0); // Move cursor to the start of the first line
          lcd.print("Without a doubt");
        }
        
        if (magic_8_random == 3)  {
          lcd.setCursor(1,0); // Move cursor to 1st char on the first line
          Serial.println("Yes definitely");
          lcd.print("Yes definitely");
        }
        
        if (magic_8_random == 4)  {
          Serial.println("You may rely on it");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("You may rely");
          lcd.setCursor(5,1); // Move cursor to 5th char on the second line
          lcd.print("on it");
        }
        
        if (magic_8_random == 5)  {
          Serial.println("As I see it, yes");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("As I see it");
          lcd.setCursor(6,1); // Move cursor to 6th char on the second line
          lcd.print("yes");
        }
        
        if (magic_8_random == 6)  {
          Serial.println("Most likely");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("Most likely");
        }
        
        if (magic_8_random == 7)  {
          Serial.println("Outlook good");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("Outlook good");
        }
        
        if (magic_8_random == 8)  {
          Serial.println("Yes");
          lcd.setCursor(6,0); // Move cursor to 6th char on the first line
          lcd.print("Yes");
        }
        
        if (magic_8_random == 9)  {
          Serial.println("Signs point to yes");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("Signs point");
          lcd.setCursor(5,1); // Move cursor to 5th char on the second line
          lcd.print("to yes");
        }
        
        if (magic_8_random == 10)  {
          Serial.println("Reply hazy try again");
          lcd.setCursor(3,0); // Move cursor to 3rd char on the first line
          lcd.print("Reply hazy");
          lcd.setCursor(3,1); // Move cursor to 3rd char on the second line
          lcd.print("try again");
        }
        
        if (magic_8_random == 11)  {
          Serial.println("Ask again later");
          lcd.setCursor(3,0); // Move cursor to 3rd char on the first line
          lcd.print("Ask again");
          lcd.setCursor(5,1); // Move cursor to 5th char on the second line
          lcd.print("later");
        }
        
        if (magic_8_random == 12)  {
          Serial.println("Better not tell you now");
          lcd.setCursor(0,0); // Move cursor to the start of the first line
          lcd.print("Better not tell");
          lcd.setCursor(3,1); // Move cursor to 3rd char on the second line
          lcd.print("you now");
        }
        
        if (magic_8_random == 13)  {
          Serial.println("Cannot predict now");
          lcd.setCursor(1,0); // Move cursor to 1st char on the first line
          lcd.print("Cannot predict");
          lcd.setCursor(6,1); // Move cursor to 6th char on the second line
          lcd.print("now");
        }
        
        if (magic_8_random == 14)  {
          Serial.println("Concentrate and ask again");
          lcd.setCursor(1,0); // Move cursor to 1st char on the first line
          lcd.print("Concentrate and");
          lcd.setCursor(3,1); // Move cursor to 3rd char on the second line
          lcd.print("ask agiain");
        }
        
        if (magic_8_random == 15)  {
          Serial.println("Don't count on it");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("Don't count");
          lcd.setCursor(5,1); // Move cursor to 5th char on the second line
          lcd.print("on it");
        }
        
        if (magic_8_random == 16)  {
          lcd.setCursor(1,0); // Move cursor to 1st char on the first line
          Serial.println("My reply is no");
          lcd.print("My reply is no");
        }
        
        if (magic_8_random == 17)  {
          Serial.println("My sources say no");
          lcd.setCursor(3,0); // Move cursor to 3rd char on the first line
          lcd.print("My sources");
          lcd.setCursor(5,1); // Move cursor to 5th char on the second line
          lcd.print("say no");
        }
        
        if (magic_8_random == 18)  {
          Serial.println("Outlook not so good");
          lcd.setCursor(2,0); // Move cursor to 2nd char on the first line
          lcd.print("Outlook not");
          lcd.setCursor(4,1); // Move cursor to 4th char on second line
          lcd.print("so good");
        }
        
        if (magic_8_random == 19)  {
          Serial.println("Very doubtful");
          lcd.setCursor(1,0); // Move cursor to second char on first line
          lcd.print("Very doubtful");
        }        

}

void playSong(const char * track)  // Takes the song_# pasted from the if statements below
{
  player.play_P(track, octave);  // Passes the song_# and octave to the player
}
