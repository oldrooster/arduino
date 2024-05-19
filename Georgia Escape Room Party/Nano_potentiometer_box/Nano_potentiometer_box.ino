#include <SoftwareSerial.h>


int pot1 = A1;
int pot2 = A2;
int pot3 = A3;
int pot4 = A4;
int relayPin = 2;
int rxPin = 3;
int txPin = 4;
int greenLed = 5;
int potVal1;
int potVal2;
int potVal3; 
int potVal4;
int delayTime = 250;
int maxValue = 748; //(750 as using 3.3v pin on nano)
int potIntervals = 4; // how many intervals to carve pot's into
int secretCode[] = {1,3,0,2};

bool playing = false;
unsigned long playTime;
int songTime = 5000;

SoftwareSerial mp3 (txPin,rxPin); //-> SoftwareSerial mp3 (RX_Pin, TX_Pin);
#define CMD_PLAY_INDEX        0X03    //-> Play with index.
#define CMD_SEL_DEV           0X09    //-> Select device.
#define DEV_TF                0X02    //-> 02 for TF card.
static int8_t Send_buf[8] = {0}; //-> Buffer to send commands. 

bool debug = true;
void setup() {
  // put your setup code here, to run once:
  pinMode(pot1,INPUT);
  pinMode(pot2,INPUT);
  pinMode(pot3,INPUT);
  pinMode(pot4,INPUT);
  pinMode(relayPin,OUTPUT);
  pinMode(greenLed,OUTPUT);
  digitalWrite(greenLed,LOW);
  if (debug)
  {
    Serial.begin(9600);
  }
  mp3.begin(9600);
  delay(500);
  sendCommand(CMD_SEL_DEV, 0, DEV_TF); // Select SD Card
  delay(500); 

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
    playSound();
    digitalWrite(relayPin,LOW);
    digitalWrite(greenLed,HIGH);
  }
  else
  {
    digitalWrite(relayPin,HIGH);
    digitalWrite(greenLed,LOW);
    playing = false;
  }
  delay(delayTime);
}

void sendCommand(byte command){
  sendCommand(command, 0, 0);
}

void sendCommand(byte command, byte dat1, byte dat2){
  delay(20);
  Send_buf[0] = 0x7E;    //-> Every command should start with $(0x7E)
  Send_buf[1] = 0xFF;    //-> Version information
  Send_buf[2] = 0x06;    //-> The number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //-> Such as PLAY and PAUSE and so on
  Send_buf[4] = 0x01;    //-> 0x00 = not feedback, 0x01 = feedback
  Send_buf[5] = dat1;    //-> data1
  Send_buf[6] = dat2;    //-> data2
  Send_buf[7] = 0xEF;    //-> Ending byte of the command
  for (uint8_t i = 0; i < 8; i++){
    mp3.write(Send_buf[i]) ;
  }
}

void playSound() {
  if (!playing)
  {
    sendCommand(CMD_PLAY_INDEX, 0, 1); //Play first song Index 1);
    playing = true;
    playTime = millis();
  }
}