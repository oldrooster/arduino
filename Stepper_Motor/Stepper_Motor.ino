#include <Stepper.h>
int stepsPerRevolution = 2048;
int motSpeed = 10;
int dt=5;
int buttonPin = 7;
int buttonValNew;
int buttonValOld = 1;
int direction = 1;

Stepper myStepper(stepsPerRevolution, 8,10,9,11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  myStepper.setSpeed(motSpeed);
  pinMode(buttonPin,INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  buttonValNew = digitalRead(buttonPin);
  if (buttonValNew == 0 && buttonValOld ==1)
  {
    direction = direction * -1;
    Serial.println("Change Direction");
  }
  myStepper.step(direction);
  buttonValOld = buttonValNew;    
}
