#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <PSEBoard.h>
#include <GPS.h>
#include <common.h>

#include "simGPS.h"

SoftwareSerial port(GPS_RX,GPS_TX);

void setup(){

  #ifdef DEBUG
    Serial1.begin(115200) ;
  #else
    Serial.begin(115200) ;
    Serial.begin(115200);
    while(!Serial);
  #endif
  delay(2000);

}

void loop(){

  
  // getGGAData();
  // getRMCData();
  // getGSAData();
  // getGSVData();
  // getVTGData();
  // Serial.println(nmeaData);
  Serial.println("Parsing...");
  gpsParseData(nmeaData);
  Serial.println("OK...");

  while(1);
  

}
