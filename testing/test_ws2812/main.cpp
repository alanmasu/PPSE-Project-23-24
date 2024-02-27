#include <Arduino.h>
#include <PSEBoard.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 8
#define BRIGHTNESS 50
#define ANGLE_STEP (360.0 / LED_COUNT)
#define HALF_ANGLE_STEP (ANGLE_STEP / 2.0)
#define MAGNETOMETER_CALIBRATION_LED_TIME 500

Adafruit_NeoPixel strip(LED_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);
const uint32_t LED_RED = strip.Color(255, 0, 0);
const uint32_t LED_GREEN = strip.Color(0, 255, 0);
const uint32_t LED_VIOLET = strip.Color(200, 0, 255);
const uint32_t LED_OFF = strip.Color(0, 0, 0);

bool blink = false;
unsigned long long t_magnetometer_calibration_led = 0, t1 = 0;

void setWS2812(float angle) {
  strip.fill(LED_OFF, 0, LED_COUNT);
  if(angle > HALF_ANGLE_STEP*15 || angle <= HALF_ANGLE_STEP) {
    strip.setPixelColor(7, LED_VIOLET);
    strip.setPixelColor(0, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP && angle <= HALF_ANGLE_STEP*3) {
    strip.setPixelColor(0, LED_VIOLET);
    strip.setPixelColor(1, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP*3 && angle <= HALF_ANGLE_STEP*5) {
    strip.setPixelColor(1, LED_VIOLET);
    strip.setPixelColor(2, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP*5 && angle <= HALF_ANGLE_STEP*7) {
    strip.setPixelColor(2, LED_VIOLET);
    strip.setPixelColor(3, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP*7 && angle <= HALF_ANGLE_STEP*9) {
    strip.setPixelColor(3, LED_VIOLET);
    strip.setPixelColor(4, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP*9 && angle <= HALF_ANGLE_STEP*11) {
    strip.setPixelColor(4, LED_VIOLET);
    strip.setPixelColor(5, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP*11 && angle <= HALF_ANGLE_STEP*13) {
    strip.setPixelColor(5, LED_VIOLET);
    strip.setPixelColor(6, LED_VIOLET);
  }
  else if(angle > HALF_ANGLE_STEP*13 && angle <= HALF_ANGLE_STEP*15) {
    strip.setPixelColor(6, LED_VIOLET);
    strip.setPixelColor(7, LED_VIOLET);
  }
  strip.show();
}

void ledShowFix(bool fix) {
  if(fix) {
    strip.fill(LED_GREEN, 0, LED_COUNT);
  } else {
    strip.fill(LED_RED, 0, LED_COUNT);
  }
  strip.show();
}

void ledShowCalibration(bool calibrated, bool magnetometer_calibrating) {
  if(millis() >= t_magnetometer_calibration_led) {
    blink = !blink;
    t_magnetometer_calibration_led = millis() + MAGNETOMETER_CALIBRATION_LED_TIME;
  }
  if(calibrated) {
    strip.fill(LED_GREEN, 0, LED_COUNT);
  } else {
    if(magnetometer_calibrating) {
      if(blink) {
        strip.fill(LED_RED, 0, LED_COUNT);
      } else {
        strip.fill(LED_OFF, 0, LED_COUNT);
      }
    } else {
      strip.fill(LED_RED, 0, LED_COUNT);
    }
  }
  strip.show();
}

void setup() {
  Serial.begin(115200);

  while(!Serial);
  delay(2000);
  Serial.println("WS2812 Test code started...");
  strip.begin();
  strip.show();
  strip.setBrightness(BRIGHTNESS);
  t_magnetometer_calibration_led = millis();
  t1 = millis() + 5000;
  ledShowCalibration(true, false);
  delay(2000);
}

bool fix = false;

void loop() {
  /*for(unsigned int i = 0; i <= 360; i++) {
    setWS2812((float)i);
    delay(10);
    Serial.print("Angle: ");
    Serial.print(i);
    Serial.println(" degree");
  }*/
  if(millis() >= t1) {
    ledShowCalibration(false, true);
  } else {
    ledShowCalibration(false, false);
  }
}