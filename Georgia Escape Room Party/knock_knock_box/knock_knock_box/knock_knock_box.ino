int knockPin = A0;
int knockLed = 6;
int failLed = 5;
int successLed = 4;
int relayPin = 2;
int flashTime = 200; //LED Flash time
int knockVal = 0;
int knockThreshold = 20;
int relayTime = 250; // keep LOW or electronic lock will burn out!

int numberOfKnocks = 2; //how many consecutive knocks
int waitTime = 700; //ms to wait until knock fails. (add to min Time);
int minTime = 300; //minimum time to wait between knocks;
bool knockStart = false; // have knocks started;
int knockCount = 0; //no. knocks registered
int lastKnock; //time of last knock
int currentTime;
bool debugOn = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(knockPin,INPUT);
  pinMode(knockLed,OUTPUT);
  pinMode(failLed,OUTPUT);
  pinMode(successLed,OUTPUT);
  pinMode(relayPin,OUTPUT);
  if (debugOn)
  {
       Serial.begin(9600);
  }
 
}

void loop() {

  knockVal = analogRead(knockPin);
  if (debugOn)
  {
      Serial.println(knockVal);
  }
  
  if (knockVal > knockThreshold)
  {
    if (debugOn)
    {
      Serial.print("KnockVal: ");
      Serial.println(knockVal);
     // delay(2000);
    }
    digitalWrite(knockLed,HIGH);
    delay(minTime);
    if (!knockStart) //first knock
    {
      knockStart = true;
      lastKnock = millis();
      knockCount = 1;
    }
    else
    {
       knockCount++;
       lastKnock = millis();
    }
    if (debugOn)
    {
      Serial.println(lastKnock);
    }
  }
  digitalWrite(knockLed,LOW);
  
  if (knockStart){
    currentTime = millis();
    if (currentTime - lastKnock > waitTime)
    {
      if (knockCount == numberOfKnocks)
      {
      //Unlock!!
      knockStart = false; // reset knock counter
      knockCount = 0;
      blink(successLed,2);
      openDoor();
      }
      else
      {
        knockStart = false; // reset knock counter
        knockCount = 0;
        blink(failLed,2);
      }
    }
  }
}


int blink(int led,int blinks){
  
  for (int i=1;i <= blinks; i++)
  {
    digitalWrite(led,HIGH);
    delay(flashTime);
    digitalWrite(led,LOW);
    delay(flashTime);
  }
}
int openDoor()
{
  digitalWrite(relayPin,HIGH);
  delay(relayTime);
  digitalWrite(relayPin,LOW);
}

