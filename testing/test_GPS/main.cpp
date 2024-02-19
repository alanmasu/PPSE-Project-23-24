#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <PSEBoard.h>
#include <GPS.h>
#include <common.h>

#include "simGPS.h"

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

const int maxDataLenght = 600;

void setup(){

  Serial2.setRX(GPS_RX);
  Serial2.setTX(GPS_TX);
  pinMode(GPS_EN,OUTPUT);

  #ifdef DEBUG
    Serial1.begin(115200) ;
  #else
    //Serial.begin(115200) ;
    Serial2.begin(9600);
    while(!Serial);
  #endif
  
  digitalWrite(GPS_EN,LOW);

}

void loop(){

  String infoGPS[12];
  char *sentence;
  // int pos_termine=0;
  // int M=600;
  // String buffer[M];

  //PRINTF("%d",Serial2.available());
  //Controllo se ci sono caratteri
  while(Serial2.available()>0){
    //Pacchetto stringhe da passare
    for(int i=0;i<12;++i){
      String sentence = Serial2.readStringUntil('\n');
      //PRINTF("%s",sentence);
      infoGPS[i]=sentence;
    }
  }

  PRINTF("\n\nINIZIO PARSING VARIE STRINGHE\n");

  for(int i=0;i<12;++i){
    PRINTF("\nParsing...");
    gpsParseData(infoGPS[i].c_str());
    PRINTF("\nOK...\n");
  }
  PRINTF("\nFine ciclo\n\n");

  // PRINTF("\nParsing...");
  //gpsParseData(sentence);
  // PRINTF("OK...\n");

}
