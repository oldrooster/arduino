#include <WiFi.h>


// Networking details **************************************
IPAddress staticIP(10, 0, 2, 248); // ESP32 static IP
IPAddress gateway(10, 0, 2, 1);  
IPAddress subnet(255, 255, 255, 0); 
IPAddress primaryDNS(10, 0, 2, 5); 
IPAddress secondaryDNS(1, 1, 1, 1);  
const char* ssid = "iotnet";
const char* password = "ThreeDeadlyWombats";
String ipAddress;
WiFiServer server(80);
String header;

// PIN  details **************************************
int relayPin = 17;
int dt = 200;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {
  // put your setup code here, to run once:
   Serial.begin(9600);
   pinMode(relayPin,OUTPUT);
   digitalWrite(relayPin,LOW);
   ipAddress = initWiFi();
    Serial.println(ipAddress);
   server.begin();

}

void loop() {
  // put your main code here, to run repeatedly:
  
  doWebStuff();

}

String initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  //Set Static IP
  if(!WiFi.config(staticIP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("Failed to configure Static IP");
  } else {
    Serial.println("Static IP configured!");
  }
  return WiFi.localIP().toString();
}

void relay(int pin, bool open)
{
  if (open)
  {
    digitalWrite(pin,HIGH);
    Serial.println("Open"); 
  }
  else
  {
    digitalWrite(pin,LOW);
    Serial.println("Close"); 
  }
}

void blink(int pin, int blinks)
{
  for (int i = 1; i <= blinks; i++)
  {
    digitalWrite(pin,HIGH);
    delay(dt);
    digitalWrite(pin,LOW);
    delay(dt);
  }
}

void doWebStuff(){
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {
  currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Do the action!
            if (header.indexOf("GET /open") >= 0) {
              relay(relayPin,1);
            }
            if (header.indexOf("GET /close") >= 0) {
              relay(relayPin,0);
            }
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}