int lightPin = A0;
int lightVal;
int dv = 250;
int redPin = 7;
int greenPin = 8;
void setup() {
  // put your setup code here, to run once:
  pinMode(lightPin,INPUT);
  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  lightVal = analogRead(lightPin)*(255./1023.);
  Serial.println(lightVal);
  
  if (lightVal <= 150)
  {
    digitalWrite(redPin,HIGH);
    digitalWrite(greenPin,LOW);
  }
  else
  {
    digitalWrite(greenPin,HIGH);
    digitalWrite(redPin,LOW);
  }
  delay(dv);
}
