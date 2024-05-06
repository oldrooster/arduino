
#include <Keypad.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C for 128x36
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
// On an arduino UNO:       A4(SDA), A5(SCL)

int redPin = 2;
int greenPin = 3;
const String secretCode = "57329";
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
byte rowPins[ROWS] = {11, 10, 9, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 6, 5}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  pinMode(greenPin, OUTPUT);
  pinMode(greenPin, OUTPUT);

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
  delay(2000);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.clearDisplay();
}
  
void loop(){
  char key = customKeypad.getKey();
  
  if (key){
    if (clearNextKeypress)
    {
      display.clearDisplay();
      display.setCursor(0,0);
      clearNextKeypress = false;
    }
    if (key == '#')
    {
      clearNextKeypress = true;
      display.println(key);
      Serial.print(code);
      if (code == secretCode)
      {
        display.println(F("SUCCESS"));
        display.display();
        blink(greenPin,2);
      }
      else
      {
        display.println(F("WRONG CODE"));
        display.display();
        blink(redPin,2);

      }
      code ="";
    }
    else
    {
      code += key;
      display.print(key);
      display.display();
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
