// ESP32 → Panel
// Panel Pin	ESP32 GPIO  Colour Cable

// G1	        26          1-Green
// GND        GND         1-Yellow
// G2	        12          1-Orange
// GND        GND         1-Red
// B	        19          1-Brown
// D	        17          1-Black
// LAT	      4           1-White
// GND	      GND         1-Grey

// R1	        25          2-Orange
// B1	        27          2-Red
// R2	        14          2-Brown
// B2	        13          2-Black
// A	        23          2-White
// C	        5           2-Grey
// CLK	      16          2-Purple
// OE	        15          2-Blue





#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define PANEL_WIDTH 64
#define PANEL_HEIGHT 32

MatrixPanel_I2S_DMA *display = nullptr;

unsigned long startMillis;
unsigned long durationSeconds = 60 * 60;  // 60 minutes

void setup() {
  HUB75_I2S_CFG mxconfig(PANEL_WIDTH, PANEL_HEIGHT, 1);
  display = new MatrixPanel_I2S_DMA(mxconfig);
  
  display->begin();
  display->setBrightness8(150);

  startMillis = millis();
}

void loop() {

  unsigned long elapsed = (millis() - startMillis) / 1000;

  if (elapsed > durationSeconds) {
    elapsed = durationSeconds;
  }

  unsigned long remaining = durationSeconds - elapsed;

  int minutes = remaining / 60;
  int seconds = remaining % 60;

  char timeString[6];
  sprintf(timeString, "%02d:%02d", minutes, seconds);

  display->clearScreen();

  display->setTextSize(2);         // Bigger text
  display->setTextWrap(false);

  // Center roughly for 64x32 panel
  display->setCursor(4, 9);

  display->setTextColor(display->color565(255, 0, 255));
  display->print(timeString);

  delay(200);
}
