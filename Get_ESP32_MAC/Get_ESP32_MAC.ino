#include <WiFi.h>
#define LED 2

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

  flash();
}
 
void loop(){
  
}

void flash()
{
  delay(500);
  digitalWrite(LED,HIGH);
  delay(500);
  digitalWrite(LED,LOW);
}