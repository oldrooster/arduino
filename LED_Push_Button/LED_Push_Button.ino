
//        ________
//       /   _1   \   --> 1 - Switch Pin (NC)
//      |    _2    |  --> 2 - Switch Pin (NO)
//      \ |3 _4 |5 /  --> 3 - LED Pin
//       \________/   --> 4 & 5 - Ground


int switchPin = 25;
int ledPin = 26;
int switchVal;
void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin,OUTPUT);
  pinMode(switchPin,INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchVal = digitalRead(switchPin);
  //Serial.println(switchVal);

  if (switchVal ==1)
  {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }

}
