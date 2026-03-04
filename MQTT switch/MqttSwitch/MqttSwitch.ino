
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

// WiFi Creds
const char* ssid = "iotnet";
const char* password = "ThreeDeadlyWombats";

// MQTT Settings
const char* mqtt_server = "mqtt.cbf.nz";   // broker IP or hostname
const int   mqtt_port   = 1883;
const char* mqtt_topic  = "escape_room/prop_test";
const char* mqtt_user = "mqttuser";
const char* mqtt_pass = "monkeyelephantsbad";
String mqtt_client_id = "prop-" + WiFi.macAddress();
WiFiClient espClient;
PubSubClient client(espClient);


// Switch Settings
int switchPin = 25;
int switchVal;
int preVal = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(switchPin,INPUT_PULLUP);
  Serial.begin(9600);
  delay(1000);
  Serial.println("starting");
  
  connectToWifi();

  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long lastChange = 0;

  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  switchVal = digitalRead(switchPin);
  
  if (switchVal != preVal && millis() - lastChange > 50) {
      lastChange = millis(); 
      preVal = switchVal;
      Serial.println(switchVal);

      //Json Payload
      StaticJsonDocument<128> doc;
      doc["switch"] = preVal;
      doc["timestamp"] = millis();
      char payload[128];
      serializeJson(doc, payload);

      //pulish topic
      client.publish(mqtt_topic, payload);
      Serial.println("Message published");
  }

}

void connectToWifi()
{
  WiFi.mode(WIFI_STA);          // Station mode (client)
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void connectMQTT() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT... ");

    if (client.connect(mqtt_client_id.c_str(), mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 1 second");
      delay(1000);
    }
  }
}


