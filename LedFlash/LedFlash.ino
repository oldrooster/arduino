#define LED_PIN 8
//#define BUTTON_PIN 7

void setup() {
  pinMode(LED_PIN, OUTPUT);
//  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(2000);
  digitalWrite(LED_PIN, LOW);
  delay(200); 
}