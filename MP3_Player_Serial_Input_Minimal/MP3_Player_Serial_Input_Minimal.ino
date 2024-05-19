/*
YX5300 MP3 Player Module      Arduino Uno
        RX <---------------------> 7
        TX <---------------------> 6
       VCC <---------------------> 5V
       GND <---------------------> GND
-------------------------------------------------------------------
*/
int rxPin = 2;
int txPin = 3;
int songTime = 5000;
bool playing = false;
unsigned long playTime;

int triggerPin = 12;
int echoPin = 11;
unsigned long  pingTravelTime;
int songDistance = 4000;

unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 250;  // interval at which to blink (milliseconds)

#include <SoftwareSerial.h>
SoftwareSerial mp3 (txPin,rxPin); //-> SoftwareSerial mp3 (RX_Pin, TX_Pin);

// ------------------------------------------------------- The byte commands are based on the table contained in the MP3 Player Module Document.
// Link to download the MP3 Player Module Document is available in the video description.

#define CMD_PLAY_INDEX        0X03    //-> Play with index.
#define CMD_SEL_DEV           0X09    //-> Select device.
#define DEV_TF                0X02    //-> 02 for TF card.
// Select device : 7E FF 06 09 00 00 02 EF -> Select storage device to TF card.
static int8_t Send_buf[8] = {0}; //-> Buffer to send commands. 
//static uint8_t ansbuf[10] = {0}; //-> Buffer to receive response.  

void setup(){

  pinMode(triggerPin,OUTPUT);
  pinMode(echoPin,INPUT);
  
  mp3.begin(9600);
  delay(500);
  Serial.begin(9600);
  sendCommand(CMD_SEL_DEV, 0, DEV_TF); // Select SD Card
  delay(500); 
  
}

void loop(){

  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    digitalWrite(triggerPin,LOW);
    delayMicroseconds(10);
    digitalWrite(triggerPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin,LOW);
    pingTravelTime=pulseIn(echoPin,HIGH);
    Serial.println(pingTravelTime);
    if (pingTravelTime < songDistance){
      playAdams();
    }
    }
  
}

void playAdams() {
  if (!playing)
  {
    sendCommand(CMD_PLAY_INDEX, 0, 1); //Play first song Index 1);
    playing = true;
    playTime = millis();
  }
  else
  {
      if ((millis() - playTime) > songTime)
      {
        
        playing = false;
      }
  }
}

// Function for sending commands.
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
