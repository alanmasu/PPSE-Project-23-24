#include <Arduino.h>
#include <PSEBoard.h>
#include <Adafruit_NeoPixel.h>
#include <PSEBoard_WS2812.h>

unsigned long long t1, t2;

PSEBoard_WS2812 pseboard;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(1000);
  pseboard.init();
  t1 = millis() + 3000;
  t2 = millis() + 10000;
  delay(2000);
}

bool fix = false;
bool all_calibrated = false;
bool magnetometer_calibrating = false;
bool disable1 = false;
bool disable2 = false;

void loop() {
  if(millis() < t1 && !disable1) {
    for(unsigned int i = 0; i <= 360; i++) {
      pseboard.ledShowDirection((float)i);
      delay(10);
    }
  }
  if(millis() >= t1 && !disable2) {
    disable1 = true;
    pseboard.ledOffShow();
    pseboard.ledShowFix(fix);
    if(!fix) {
      t1 = millis() + 2000;
      fix = !fix;
    }
  }
  if(disable1 && disable2) {
    pseboard.ledShowCalibration(all_calibrated, magnetometer_calibrating);
  }
  if(millis() >= t2) {
    disable2 = true;
    if(!all_calibrated && !magnetometer_calibrating) {
      magnetometer_calibrating = true;
      t2 = millis() + 4000;
    }
    else {
      if(!all_calibrated && magnetometer_calibrating) {
        all_calibrated = true;
        t2 = millis() + 2000;
      }
    }
  }
}