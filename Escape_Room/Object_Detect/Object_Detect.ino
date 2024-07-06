
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <SoftwareSerial.h>

// PN532 PINS
#define PN532_SCK  (7) //Grey
#define PN532_MOSI (5) //Purple
#define PN532_SS   (4) //Orange
#define PN532_MISO (6) //White

//YX6300 MP3 PINS
#define RX_PIN     (9) //Purple
#define TX_PIN     (8) //Orange

//ESP32 SIGNAL PINS
#define SUCCESS_PIN (3) //send HIGH when object detected
#define TRACK5_PIN (A1)
#define TRACK4_PIN (A2)
#define TRACK3_PIN (A3)
#define TRACK2_PIN (A4)
#define TRACK1_PIN (A5)

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

bool pauseReading = false;
unsigned long lastRead;
unsigned long currentTime;
unsigned long waitTime = 30000;
int rfidTimeout = 1000;

// ********** Serial MP3 Commands - minimal *******************
// Serial2 used - MP3 RX Pin to GPIO 17(TX) TX PIN GPIO 16(RX)
#define CMD_PLAY_INDEX        0X03    //-> Play with index.
#define CMD_SEL_DEV           0X09    //-> Select device.
#define DEV_TF                0X02    //-> 02 for TF card.
// Select device : 7E FF 06 09 00 00 02 EF -> Select storage device to TF card.
static int8_t Send_buf[8] = {0}; //-> Buffer to send commands. 
SoftwareSerial mp3 (RX_PIN,TX_PIN); //-> SoftwareSerial mp3 (RX_Pin, TX_Pin);

void setup(void) {
  Serial.begin(115200);
  pinMode(SUCCESS_PIN,OUTPUT);
  pinMode(TRACK1_PIN,INPUT);
  pinMode(TRACK2_PIN,INPUT);
  pinMode(TRACK3_PIN,INPUT);
  pinMode(TRACK4_PIN,INPUT);
  pinMode(TRACK5_PIN,INPUT);
  while (!Serial) delay(10); // for Leonardo/Micro/Zero

  Serial.println("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);

  Serial.println("Waiting for an ISO14443A Card ...");

  // *********************************** Config MP3
  mp3.begin(9600); // MP3 Player
  delay(500); // wait half a second for MP3 player
  sendCommand(CMD_SEL_DEV, 0, DEV_TF); // Select SD Card
  delay(500); // wait another half a second for MP3 player
  //playSound(3);
}

  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

void loop(void) {

  int track = 0;
  
  if (digitalRead(TRACK1_PIN) == 1)
  {
    track = 1;
  }
  if (digitalRead(TRACK2_PIN) == 1)
  {
    track = 2;
  }
  if (digitalRead(TRACK3_PIN) == 1)
  {
    track = 3;
  }
  if (digitalRead(TRACK4_PIN) == 1)
  {
    track = 4;
  }
  if (digitalRead(TRACK5_PIN) == 1)
  {
    track = 5;
  }
  if (track != 0)
  {
    Serial.print(" Track: ");Serial.println(track);
    playSound(track);
    delay(100);
  }
  

  // Wait for an NTAG203 card.  When one is found 'uid' will be populated with
  // the UID, and uidLength will indicate the size of the UUID (normally 7)
  if (!pauseReading) 
  {
    success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength,rfidTimeout);

    if (success) {
      // // Display some basic information about the card
      Serial.print("Found an ISO14443A card: ");
      // Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
      // Serial.print("  UID Value: ");
      nfc.PrintHex(uid, uidLength);
      // Serial.println("");   
      Serial.flush();
      pauseReading = true;
      lastRead = millis();
      analogWrite(SUCCESS_PIN,192);

    }
    else
    {
      Serial.println("Timed out trying to read");
      analogWrite(SUCCESS_PIN,0);

      //nfc.begin();
    }
  }
  else
  {
    currentTime = millis();

    if (currentTime - lastRead > waitTime)
    {
      pauseReading = false;
    }
    //Serial.print("myTime:   ");Serial.println(currentTime - lastRead);
     delay(50);
  }
  
}

void playSound(int track)
{
  Serial.print("Playing Track: ");
  Serial.println(track);
  sendCommand(CMD_PLAY_INDEX, 0, track); 
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