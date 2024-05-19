
#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "pitches.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x36
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const char* ssid = "iotnet";
const char* password = "ThreeDeadlyWombats";
const String doorOpenUrl = "http://10.0.2.248/close";
const String doorCloseUrl = "http://10.0.2.248/open";
const String secretCode = "24123";
const String lockCode = "99999";

const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns

String code = "";
int dt = 200;
bool clearNextKeypress = false;

//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = {14, 27, 26, 25}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {33, 32, 18}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 


int buzzerPin = 17;
int melody[] = {
  NOTE_C5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, 0, NOTE_B4, NOTE_C5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

String ipAddress;

IPAddress staticIP(10, 0, 2, 247); // ESP32 static IP
IPAddress gateway(10, 0, 2, 1);  
IPAddress subnet(255, 255, 255, 0); 
IPAddress primaryDNS(10, 0, 2, 5); 
IPAddress secondaryDNS(1, 1, 1, 1);  


void setup(){
  Serial.begin(9600);
  // pinMode(redPin, OUTPUT);
  // pinMode(greenPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);


    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.display();
  delay(2000); // Pause for 2 seconds
  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  display.display();
  ipAddress = initWiFi();
  //delay(2000);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  resetScreen();
  display.println(ipAddress);
  display.display();
  clearNextKeypress = true;
}
  
void loop(){
  char key = customKeypad.getKey();
  
  if (key){
    Serial.print("Key Pressed:");
    Serial.println(key);
    if (clearNextKeypress)
    {
      resetScreen();
      clearNextKeypress = false;
    }
    if (key == '#')
    {
      clearNextKeypress = true;
      display.println(key);
      Serial.print(code);
      keyBeep();
      if (code == secretCode)
      {
        display.println(F("SUCCESS"));
        display.display();
        successTone();
        //blink(greenPin,2);
        sendUnlockRequest();
      }
      else 
      {
        if (code == lockCode)
        {
          display.println(F("LOCKED"));
          display.display();
          keyBeep();
          keyBeep();
          //blink(greenPin,2);
          sendLockRequest();
        }
        else
        {
          display.println(F("WRONG CODE"));
          display.display();
          //blink(redPin,2);
          failureTone();
        }
      }
      code ="";
    }
    else
    {
      code += key;
      display.print(key);
      display.display();
      keyBeep();
    }
  }
}

void blink(int pin, int blinks)
{
  for (int i = 1; i <= blinks; i++)
  {
    digitalWrite(pin,HIGH);
    delay(dt);
    digitalWrite(pin,LOW);
    delay(dt);
  }
}

//Buzzer noises
void keyBeep()
{
  tone(buzzerPin,NOTE_D5);
  delay(200);
  noTone(buzzerPin);
}

void successTone()
{
  delay(250);
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 750 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}
void failureTone()
{
  delay(250);
  tone(buzzerPin,NOTE_G4);
  delay(250);
  tone(buzzerPin,NOTE_C4);
  delay(500);
  noTone(buzzerPin);
}

String initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  //Set Static IP
  if(!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }
  return WiFi.localIP().toString();;
}

void sendUnlockRequest()
{
  HTTPClient http;
  http.begin(doorOpenUrl);
  int httpCode = http.GET();
}
void sendLockRequest()
{
  HTTPClient http;
  http.begin(doorCloseUrl);
  int httpCode = http.GET();

}
void resetScreen()
{
      display.clearDisplay();
      display.setCursor(0,0);
      display.println(F(" "));
      display.display();
}
