#include <esp_now.h>
#include <WiFi.h>
#define LED 2


// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool open;
} struct_message;
struct_message myData;

void setup(){
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  // Variable to store the MAC address
  uint8_t baseMac[6];
  
  // Get MAC address of the WiFi station interface
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  Serial.print("Station MAC (ESP-NOW Receiver): ");
  for (int i = 0; i < 5; i++) {
    Serial.printf("%02X:", baseMac[i]);
  }
  Serial.printf("%02X\n", baseMac[5]);

  flash(2);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

}
 
void loop(){
  
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Open: ");
  Serial.println(myData.open);
  Serial.println();
  flash(2);
}

void flash(int freq)
{
  for (int i=0; i<freq;i++)
  {
    delay(250);
    digitalWrite(LED,HIGH);
    delay(250);
    digitalWrite(LED,LOW);
  }
}