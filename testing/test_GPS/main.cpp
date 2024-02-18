#include <Arduino.h>
#include <Wire.h>
//#include <SoftwareSerial.h>
#include <PSEBoard.h>
#include <GPS.h>
#include <common.h>

#include "simGPS.h"

//SoftwareSerial port(GPS_RX,GPS_TX);

void setup(){

  Serial.begin(115200);
  while(!Serial);
  delay(2000);

}

void loop(){

  
  //  getGGAData();
  // getRMCData();
  // getGSAData();
  // getGSVData();
  // getVTGData();
  // Serial.println(nmeaData);
  Serial.println("Parsing...");
  gpsParseData(nmeaData);
  Serial.println("OK...");

  Serial.printf("%s\n",getGGAData()->sats);

  while(1);
  

}
