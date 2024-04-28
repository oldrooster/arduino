int knockPin = A0;
int ledPin = 2;
int delayTime = 100;
int knockVal = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(knockPin,INPUT);
  pinMode(ledPin,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(ledPin,HIGH);
  // delay(delayTime);
  // digitalWrite(ledPin,LOW);
  // delay(delayTime);
  knockVal = analogRead(knockPin);
  if (knockVal > 12)
  {
    Serial.print("KnockVal: ");
    Serial.println(knockVal);
    digitalWrite(ledPin,HIGH);
    delay(delayTime);
  }
  digitalWrite(ledPin,LOW);
}
