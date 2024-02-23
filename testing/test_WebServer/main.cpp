#include <Arduino.h>
#include <git_revision.h>
#include "PSEBoard.h"
#include "common.h"
#include <ESPModule.h>
#include <IPAddress.h>

//OLED display libraries
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH   128  // OLED display width, in pixels
#define SCREEN_HEIGHT  64   // OLED display height, in pixels

#define OLED_RESET     -1   // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C // See datasheet for Address; 0x3C 

#define ADC_BITS 12
#define ADC_MAX_VAL ((1 << ADC_BITS) - 1)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Timers 
uint64_t t0 = 0;      //Timer for the alive LED
uint64_t t1 = 0;      //Timer for temperature sensor
uint64_t t2 = 0;      //Timer for ESP UART

ApplicationRecord_t applicationRecord;
WiFiConfiguration_t wifiConfig;

ESPModule myESP(115200, Serial1, ESP_RX, ESP_TX);

void setup() {

  //Serial port for debugging purposes
  Serial.begin(115200);

  //Initialize the alive LED
  pinMode(ALIVE_LED, OUTPUT);
  pinMode(BTN_UP, INPUT);
  delay(10);

  //Wait for serial port to be ready
  #ifndef HW_DEBUG
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
  #endif
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

  myESP.begin();

}

void updateLCD(float temp, float cpuTemp){
  display.clearDisplay();
  display.setCursor(0,0);   
  display.setTextSize(1);
  display.print("CPU T: ");
  display.print(cpuTemp);
  display.println(" C\n");

  display.setTextSize(2);
  display.print("T: ");          
  display.print(temp);
  display.println(" C");
  display.display();
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
    applicationRecord.temp = (vTempSensor - 500) * 0.10;               // Temp in C (10mV/C) 
    //Get the CPU temperature
    applicationRecord.cpuTemp = analogReadTemp(3.27);
    //Print the temperature to the OLED display
    updateLCD(applicationRecord.temp, applicationRecord.cpuTemp);    
    t1 = millis();
  }

  if(millis() - t2 > 1000) {
    myESP.sendData(&applicationRecord);
    t2 = millis();
    Serial.printf("SSID: %s, IP: %d.%d.%d.%d, AP: %d\n", wifiConfig.ssid, wifiConfig.ipAddress[0], wifiConfig.ipAddress[1], wifiConfig.ipAddress[2], wifiConfig.ipAddress[3], wifiConfig.ap);
  }
  myESP.getData(wifiConfig);
}