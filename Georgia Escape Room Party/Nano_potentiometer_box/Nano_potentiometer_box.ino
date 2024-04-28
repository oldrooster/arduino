int pot1 = A1;
int pot2 = A2;
int pot3 = A3;
int pot4 = A4;
int relayPin = 2;
int potVal1;
int potVal2;
int potVal3; 
int potVal4;
int delayTime = 250;
int maxValue = 748; //(750 as using 3.3v pin on nano)
int potIntervals = 4; // how many intervals to carve pot's into
int secretCode[] = {1,3,0,2};
bool debug = true;
void setup() {
  // put your setup code here, to run once:
  pinMode(pot1,INPUT);
  pinMode(pot2,INPUT);
  pinMode(pot3,INPUT);
  pinMode(pot4,INPUT);
  pinMode(relayPin,OUTPUT);
  if (debug)
  {
    Serial.begin(9600);
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  potVal1 = abs((analogRead(pot1)*potIntervals)/maxValue);
  potVal2 = abs((analogRead(pot2)*potIntervals)/maxValue);
  potVal3 = abs((analogRead(pot3)*potIntervals)/maxValue);
  potVal4 = abs((analogRead(pot4)*potIntervals)/maxValue);
  if (potVal1 == potIntervals){
    potVal1 = potVal1 - 1;
  }
  if (potVal2 == potIntervals){
    potVal2 = potVal2 - 1;
  }
  if (potVal3 == potIntervals){
    potVal3 = potVal3 - 1;
  }
  if (potVal4 == potIntervals){
    potVal4 = potVal4 - 1;
  }
  if (debug)
  {
    Serial.print("PotVal1: ");
    Serial.print(potVal1);
    Serial.print(" PotVal2: ");
    Serial.print(potVal2);
    Serial.print(" PotVal3: ");
    Serial.print(potVal3);
    Serial.print(" PotVal4: ");
    Serial.println(potVal4);
  }
  if (potVal1 == secretCode[0] && potVal2 == secretCode[1] && potVal3 == secretCode[2] && potVal4 == secretCode[3])
  {
    if (debug)
    {
      Serial.println("SECRET CODE CORRECT: UNLOCK!");
    }
    digitalWrite(relayPin,LOW);
  }
  else
  {
    digitalWrite(relayPin,HIGH);
  }
  delay(delayTime);
  


}
