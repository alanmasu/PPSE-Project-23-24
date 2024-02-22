#include <Arduino.h>
#include <Wire.h>
//#include <SoftwareSerial.h>
#include <PSEBoard.h>
#include <GPS.h>

#include "simGPS.h"

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

//SoftwareSerial port(GPS_RX,GPS_TX);

void setup(){

  #ifdef DEBUG
  Serial1.begin(115200);
  #else
  Serial.begin(115200);
  while(!Serial);
  #endif
  delay(2000);

  Serial2.setRX(GPS_RX);
  Serial2.setTX(GPS_TX);
  Serial2.begin(9600);
  Serial1.begin(115200) ;

  pinMode(GPS_EN,OUTPUT);
  digitalWrite(GPS_EN,LOW);

  pinMode(ALIVE_LED,OUTPUT);
}

uint64_t t0 = 0;
uint8_t countpacket = 0;

void loop(){

  String infoGPS;
  
  uint16_t dt = millis() - t0;
  if(dt < 1000) {
      digitalWrite(ALIVE_LED, HIGH);
      // Serial.println("ON");
  }else if(1000 < dt && dt < 2000) {
      // Serial.println("OFF");
      digitalWrite(ALIVE_LED, LOW);
  }else if(2000 < dt){
      t0 = millis();
  }

  if(Serial2.available()>0){
  // Serial.print(Serial2.readString());
    String sentence = Serial2.readStringUntil('\n');
    if(sentence != "" && countpacket < 12){
      Serial.print("Sentence: ");
      Serial.println(sentence);
      infoGPS+=sentence;
      countpacket++;
    }
  }

  if(countpacket > 11){
    countpacket = 0;
    gpsParseData(infoGPS.c_str());
    Serial.println("Decodificato!");
  }

  // Serial.printf("%s\n",getGGAData()->sats);

  // while(1);
  

}
