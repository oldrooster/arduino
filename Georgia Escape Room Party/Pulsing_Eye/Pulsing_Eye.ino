int creepyLight = 6;
int minLight = 10;
int maxLight = 150;
int i=0;
bool reverse;
int waitTime = 4;
void setup() {
  // put your setup code here, to run once:
  pinMode(6,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(creepyLight,i);
  if (i == maxLight)
  {
    reverse = true;
  }
  if (i == minLight)
  {
    reverse = false;
  }
  if (reverse)
  {
    i = i - 1;
  }
  else
  {
    i = i + 1;
  }
  delay(waitTime);

}
