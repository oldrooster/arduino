int redLed = 9;
int blueLed = 3;
int yellowLed = 6;
int waitTime = 500;
String msg="Which LED do you want to light up?";
String msg2 = "Your chose: ";
String myColor;
void setup() {
  // put your setup code here, to run once:
  pinMode(redLed,OUTPUT);
  pinMode(yellowLed,OUTPUT);
  pinMode(blueLed,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(msg);
  while (Serial.available()==0)
  {

  }
  myColor = Serial.readString();
  Serial.print(msg2);
  Serial.println(myColor);

    myColor.toLowerCase();
    
  if (myColor=="yellow"){
    digitalWrite(yellowLed,HIGH);
    digitalWrite(blueLed,LOW);
    digitalWrite(redLed,LOW);
    Serial.println("Light up Yellow");
  }
  if (myColor=="red"){
    digitalWrite(redLed,HIGH);
    digitalWrite(yellowLed,LOW);
    digitalWrite(blueLed,LOW);
    Serial.println("Light up red");
  }
  if (myColor=="blue"){
    digitalWrite(blueLed,HIGH);
    digitalWrite(yellowLed,LOW);
    digitalWrite(redLed,LOW);
    Serial.println("Light up blue");
  }

    if (myColor!="red"){
    
    Serial.println("IT AINT RED");
  }
}
  
  
