int answer[] = {2,8,6,5};
int answerLength = 4; // Must match the no. of values in the array above.

int touchPin1 = T9; // GPIO 32
int touchPin2 = T0; // GPIO 04
int touchPin3 = T3; // GPIO 15
int touchPin4 = T4; // GPIO 13
int touchPin5 = T5; // GPIO 12
int touchPin6 = T6; // GPIO 14
int touchPin7 = T7; // GPIO 27
int touchPin8 = T8; // GPIO 33

#define greenLed  (26)
#define redLed  (23)
#define unlockPin (34)
#define track1Pin (21)
#define track2Pin (19)
#define track3Pin (18)
#define track4Pin (5)
#define track5Pin (17)
bool propLocked = true;
int unlockVal;
// ints to store values of touch sensors
int touchVal1,touchVal2,touchVal3,touchVal4,touchVal5,touchVal6,touchVal7,touchVal8;

// touch thresholds per pin (they vary depending on cable length etc.), below this value is a touch.
int th1 = 40, th2 = 35, th3 = 32, th4 = 35, th5 = 37, th6 = 36, th7 =35, th8 = 35;

int delayTime = 50;
int position = 0;
int lastCard = 0;
unsigned long lastSound = 0;
unsigned long currentTime;
unsigned long minSoundLength = 145;


int totalFails = 30; // No. of low readings on Success Pin before locking, i.e. no object near
int currentFails = totalFails;

// // ********** Serial MP3 Commands - minimal *******************
// // Serial2 used - MP3 RX Pin to GPIO 17(TX) TX PIN GPIO 16(RX)
// #define CMD_PLAY_INDEX        0X03    //-> Play with index.
// #define CMD_SEL_DEV           0X09    //-> Select device.
// #define DEV_TF                0X02    //-> 02 for TF card.
// // Select device : 7E FF 06 09 00 00 02 EF -> Select storage device to TF card.
// static int8_t Send_buf[8] = {0}; //-> Buffer to send commands. 

// ******************* ESP32 NOW Config
#include <esp_now.h>
#include <WiFi.h>
uint8_t broadcastAddress[] = {0xC8, 0x2E, 0x18, 0xC3, 0x72, 0x30}; // MAC: C8:2E:18:C3:72:30

// struct data packet to send - just a bool
typedef struct struct_message {
  bool open;
} struct_message;
struct_message myData;
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


void setup() {
  
  Serial.begin(115200);
  

  // *********************************** Config PINS

  pinMode(greenLed,OUTPUT);
  pinMode(redLed,OUTPUT);
  pinMode(unlockPin,INPUT);
  pinMode(track1Pin,OUTPUT);
  pinMode(track1Pin,OUTPUT);
  pinMode(track2Pin,OUTPUT);
  pinMode(track3Pin,OUTPUT);
  pinMode(track4Pin,OUTPUT);
  pinMode(track5Pin,OUTPUT);
  lockProp();
  // // *********************************** Config MP3
  // Serial2.begin(9600); // MP3 Player
  // delay(500); // wait half a second for MP3 player
  // sendCommand(CMD_SEL_DEV, 0, DEV_TF); // Select SD Card
  // delay(500); // wait another half a second for MP3 player
  // *********************************** Config WiFI and ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent); 
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  Serial.println("Started");
}
void lockProp()
{
  propLocked = true;
  digitalWrite(greenLed,LOW);
  digitalWrite(redLed,HIGH);
}
void unlockProp()
{
  propLocked = false;
  digitalWrite(greenLed,HIGH);
  digitalWrite(redLed,LOW);
}

void loop() {


  unlockVal = digitalRead(unlockPin);
  if (unlockVal == 1)
  {
    unlockProp();
    currentFails = 0;
  }
  else
  {
    if (currentFails >= totalFails)
    {
      lockProp();
    }
    else
    {
      currentFails++;
    }
  }

  if (!propLocked)
  {
    readtouch();
  }
  delay(delayTime);

  currentTime = millis();
  if (currentTime - lastSound > minSoundLength)
  {
    digitalWrite(track1Pin,LOW);
    digitalWrite(track2Pin,LOW);
    digitalWrite(track3Pin,LOW);
    digitalWrite(track4Pin,LOW);
    digitalWrite(track5Pin,LOW);
  }
  // for (int i = 1; i<=5;i++)
  // {
  //   playSound(i);
  //   currentTime = millis();
  //   if (currentTime - lastSound > minSoundLength)
  //   {
  //     analogWrite(soundPin,0);
  //   }
  //   delay(2000);
  // }
}


void readtouch()
{

  touchVal1 = touchRead(touchPin1);
  touchVal2 = touchRead(touchPin2);
  touchVal3 = touchRead(touchPin3);
  touchVal4 = touchRead(touchPin4);
  touchVal5 = touchRead(touchPin5);
  touchVal6 = touchRead(touchPin6);
  touchVal7 = touchRead(touchPin7);
  touchVal8 = touchRead(touchPin8);

  if (touchVal1 <= th1)
  {
    touchedCard(1);
  }
  if (touchVal2 <= th2)
  {
    touchedCard(2);
  }
  if (touchVal3 <= th3)
  {
    touchedCard(3);
  }
  if (touchVal4 <= th4)
  {
    touchedCard(4);
  }
  if (touchVal5 <= th5)
  {
    touchedCard(5);
  }
  if (touchVal6 <= th6)
  {
    touchedCard(6);
  }
  if (touchVal7 <= th7)
  {
    touchedCard(7);
  }
  if (touchVal8 <= th8)
  {
    touchedCard(8);
  }

  //uncomment to poll  values
  // String output = "1:" + String(touchVal1) + " 2:" + String(touchVal2) + " 3:" + String(touchVal3)+ " 4:" + String(touchVal4);
  // output = output + " 5:" + String(touchVal5) + " 6:" + String(touchVal6) + " 7:" + String(touchVal7)+ " 8:" + String(touchVal8);
  // Serial.println(output);

}

void solved()
{
  position = 0;
  Serial.println("SUCCESS!");

  // Send unlock over ESP-NOW
  myData.open = true;
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }

}

void playSound(int track)
{

  Serial.print("Playing Track: ");
  Serial.println(track);
  // sendCommand(CMD_PLAY_INDEX, 0, track); 
  switch (track) {
    case 1: 
      digitalWrite(track1Pin,HIGH);
      break;
    case 2:
      digitalWrite(track2Pin,HIGH);
      break;
    case 3:  
      digitalWrite(track3Pin,HIGH);
      break;
  case 4:  // Final Sound
    digitalWrite(track4Pin,HIGH);
    break;
  case 5:  // Fail Sound
    digitalWrite(track5Pin,HIGH);
    break;
  }
  lastSound = millis();
}

void touchedCard(int card)
{
  Serial.print("Card ");
  Serial.print(card);
  Serial.println(" pressed");

  if (card != lastCard)
  {
    if (answer[position] == card)
    {
      position++;
      playSound(position);
      lastCard = card;
      
      Serial.print("Right card: ");
      Serial.print(position);
      Serial.print(" of ");
      Serial.println(answerLength);

      if (position == answerLength)
      {
        solved();
      }
      
    }
    else
    {
      Serial.println("Wrong Card");
      lastCard = card;
      position = 0;
      playSound(5);
    }
  }
}

// // Function for sending commands.
// void sendCommand(byte command){
//   sendCommand(command, 0, 0);
// }

// void sendCommand(byte command, byte dat1, byte dat2){
//   delay(20);
//   Send_buf[0] = 0x7E;    //-> Every command should start with $(0x7E)
//   Send_buf[1] = 0xFF;    //-> Version information
//   Send_buf[2] = 0x06;    //-> The number of bytes of the command without starting byte and ending byte
//   Send_buf[3] = command; //-> Such as PLAY and PAUSE and so on
//   Send_buf[4] = 0x01;    //-> 0x00 = not feedback, 0x01 = feedback
//   Send_buf[5] = dat1;    //-> data1
//   Send_buf[6] = dat2;    //-> data2
//   Send_buf[7] = 0xEF;    //-> Ending byte of the command
//   for (uint8_t i = 0; i < 8; i++){
//     Serial2.write(Send_buf[i]) ;
//   }
// }