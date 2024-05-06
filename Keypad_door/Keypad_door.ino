
#include <Keypad.h>

int redPin = 2;
int greenPin = 3;
const String secretCode = "57329";
const byte ROWS = 4; //four rows
const byte COLS = 3; //four columns
String code = "";
int dt = 200;
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
}
  
void loop(){
  char key = customKeypad.getKey();
  
  if (key){
    if (key == '#')
    {
      Serial.println(code);
      if (code == secretCode)
      {
        blink(greenPin,2);
      }
      else
      {
        blink(redPin,2);
      }
      code ="";
    }
    else
    {
      code += key;
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
