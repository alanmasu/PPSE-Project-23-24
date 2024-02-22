#include <Arduino.h>
#include <git_revision.h>
#include "PSEBoard.h"
#include "common.h"
#include <IPAddress.h>

//OLED display libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//WS2812 LED libraries
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3C 

#define ADC_BITS 12
#define ADC_MAX_VAL ((1 << ADC_BITS) - 1)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Timers 
uint64_t t0 = 0;      //Timer for the alive LED
uint64_t t1 = 0;      //Timer for temperature sensor
uint64_t t2 = 0;      //Timer for ESP UART
uint64_t tLeds = 0;   //Timer for the LED strip

#define LED_COUNT  8

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 50 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);

ApplicationRecord_t applicationRecord;
WiFiConfiguration_t wifiConfig;

void setup() {

  //Serial port for debugging purposes
  Serial.begin(115200);

  //Initialize the alive LED
  pinMode(ALIVE_LED, OUTPUT);
  pinMode(BTN_UP, INPUT);
  delay(10);

  //Wait for serial port to be ready
  while(!Serial){
    uint16_t dt = millis() - t0;
    if(dt < 100) {
      digitalWrite(ALIVE_LED, HIGH);
    }else if(100 < dt && dt < 200) {
      digitalWrite(ALIVE_LED, LOW);
    }else if(200 < dt){
      t0 = millis();
    }
    // if(!digitalRead(BTN_UP)) {
    //   break;
    // }
  } 
  delay(2000);
  // while(!Serial);
  Serial.printf("Git info: %s %s\n", __GIT_COMMIT__, __GIT_REMOTE_URL__);
  Serial.printf("Built on %s at %s\n", __DATE__, __TIME__);

  //Inizializzazione dell'autoritenuta
  pinMode(BUCK_EN, OUTPUT);
  digitalWrite(BUCK_EN, HIGH);

  //Initialize the temperature sensor
  analogReadResolution(ADC_BITS);

  //Initialize the OLED display
  Wire.setSDA(I2C0_SDA);
  Wire.setSCL(I2C0_SCL);

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
  }
  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(40,30);             
  display.println("Hello PSE!");
  display.display();

  //Initialize the ESP01 pins
  pinMode(ESP_EN, OUTPUT);
  digitalWrite(ESP_EN, HIGH);

  //Initialize ESP01 Serial
  Serial1.setRX(ESP_TX);
  Serial1.setTX(ESP_RX);
  Serial1.begin(115200);

  //Initialize the WS2812 LED strip
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  // Serial.printf("Size of WiFiConfiguration_t: %d\n", sizeof(WiFiConfiguration_t));
  // Serial.printf("Size of WiFiConfiguration_t: %d\n", sizeof(wifiConfig));
}

void loop() {
  uint16_t dt = millis() - t0;
  if(dt < 1000) {
    digitalWrite(ALIVE_LED, HIGH);
  }else if(1000 < dt && dt < 2000) {
    digitalWrite(ALIVE_LED, LOW);
  }else if(2000 < dt){
    t0 = millis();
  }

  if(millis() - t1 > 1000) {
    int analog = analogRead(TEMP_SENSOR);
    float vTempSensor = analog * (3.3 / ADC_MAX_VAL) * 1000;           // V in mV
    applicationRecord.temp = (vTempSensor - 500) * 0.10;                    // Temp in C (10mV/C)  

    //Get the CPU temperature
    applicationRecord.cpuTemp = analogReadTemp(3.27);
  
    // Serial.printf("Analog read: %d,\tTEMP Voltage: %f,\tTemp: %f,\tCPU T: %f\n",analog, vTempSensor, data.temp, data.cpuTemp);
  

    //Print the temperature to the OLED display
    String tempString = String(applicationRecord.temp);
    String CPUTempString = String(applicationRecord.cpuTemp);

    display.clearDisplay();

    display.setCursor(0,0);   
    display.setTextSize(1);
    display.print("CPU T: ");
    display.print(CPUTempString);
    display.println(" C\n");

    display.setTextSize(2);
    display.print("T: ");          
    display.print(tempString);
    display.println(" C");

    // display.setTextSize(1);
    // display.print("R: ");
    // display.println(ledR);
    // display.print("G: ");
    // display.println(ledG);
    // display.print("B: ");
    // display.println(ledB);

    display.display();
    t1 = millis();
  }

  if(millis() - t2 > 1000) {
    Serial1.write((byte*)&applicationRecord, sizeof(applicationRecord));
    t2 = millis();
    Serial.printf("SSID: %s, IP: %d.%d.%d.%d, AP: %d\n", wifiConfig.ssid, wifiConfig.ipAddress[0], wifiConfig.ipAddress[1], wifiConfig.ipAddress[2], wifiConfig.ipAddress[3], wifiConfig.ap);
  }
  String str;
  while(Serial1.available()>0) {
    str = Serial1.readStringUntil('\n');
  }
  if(str != ""){
    memcpy(wifiConfig.ssid, splitString(str, ';', 0).c_str(), 32);
    wifiConfig.ssid[32] = '\0';
    String ip = splitString(str, ';', 1);
    // wifiConfig.ipAddress = IPAddress();
    wifiConfig.ipAddress.fromString(ip);
    wifiConfig.ap = splitString(str, ';', 2).toInt();
  }


  // if(millis() - tLeds > 1000) {
  //   if(!msgData.ledState) {
  //       strip.clear();
  //       strip.show();
  //   }else{
  //     for(uint8_t i = 0; i < LED_COUNT; i++) {
  //       if(i < msgData.ledCount) {
  //         strip.setPixelColor(i, strip.Color(ledR, ledG, ledB));
  //       }else{
  //         strip.setPixelColor(i, strip.Color(0, 0, 0));
  //       }
  //       strip.show();
  //     }
  //   }
  //   tLeds = millis();
  // }

}
