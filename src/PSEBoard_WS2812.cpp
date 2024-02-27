#include <Arduino.h>
#include <PSEBoard.h>
#include <Adafruit_NeoPixel.h>
#include <PSEBoard_WS2812.h>

void PSEBoard_WS2812::init() {
  strip.begin();
  globalBrightness(DEFAULT_BRIGHTNESS);
  ledOff();
  strip.show();
  t_magnetometer_calibration_led = 0;
  blink = false;
}

void PSEBoard_WS2812::ledShowDirection(float angle) {
  ledOff();
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

void PSEBoard_WS2812::ledShowFix(bool fix) {
  if(fix) {
    ledFill(LED_GREEN);
  } else {
    ledFill(LED_RED);
  }
  strip.show();
}

void PSEBoard_WS2812::ledShowCalibration(bool calibrated, bool magnetometer_calibrating) {
  if(millis() >= t_magnetometer_calibration_led) {
    blink = !blink;
    t_magnetometer_calibration_led = millis() + MAGNETOMETER_CALIBRATION_LED_TIME;
  }
  if(calibrated) {
    ledOff();
  } else {
    if(magnetometer_calibrating) {
      if(blink) {
        ledFill(LED_YELLOW);
      } else {
        ledOff();
      }
    } else {
      ledFill(LED_YELLOW);
    }
  }
  strip.show();
}

void PSEBoard_WS2812::globalBrightness(uint8_t brightness) {
  strip.setBrightness(brightness);
}

void PSEBoard_WS2812::ledOffShow() {
  ledOff(true);
}

void PSEBoard_WS2812::ledOff(bool show) {
  strip.fill(LED_OFF, 0, LED_COUNT);
  if(show) {
    strip.show();
  }
}

void PSEBoard_WS2812::ledFill(uint32_t color, bool show) {
  strip.fill(color, 0, LED_COUNT);
  if(show) {
    strip.show();
  }
}