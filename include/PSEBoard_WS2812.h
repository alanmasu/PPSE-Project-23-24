#include <Arduino.h>
#include <PSEBoard.h>
#include <Adafruit_NeoPixel.h>

#define LED_COUNT 8
#define DEFAULT_BRIGHTNESS 50
#define ANGLE_STEP (360.0 / LED_COUNT)
#define HALF_ANGLE_STEP (ANGLE_STEP / 2.0)
#define MAGNETOMETER_CALIBRATION_LED_TIME 250

const uint32_t LED_RED = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t LED_GREEN = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t LED_BLUE = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t LED_YELLOW = Adafruit_NeoPixel::Color(255, 255, 0);
const uint32_t LED_VIOLET = Adafruit_NeoPixel::Color(200, 0, 255);
const uint32_t LED_OFF = Adafruit_NeoPixel::Color(0, 0, 0);

class PSEBoard_WS2812 {
    public:
        void init();
        void globalBrightness(uint8_t brightness);
        void ledShowDirection(float angle);
        void ledShowFix(bool fix);
        void ledShowCalibration(bool calibrated, bool magnetometer_calibrating = false);
        void ledOffShow();
        void ledFill(uint32_t color, bool show = false);

    private:
        void ledOff(bool show = false);
        bool blink;
        unsigned long long t_magnetometer_calibration_led;
        Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);
};