#include <Arduino.h>
#include <Wire.h>
#include <PSEBoard.h>

#ifndef PRINTF 
  #ifdef DEBUG
      #define PRINTF(...) Serial1.printf(__VA_ARGS__)
  #else
     #define PRINTF(...) Serial.printf(__VA_ARGS__)
  #endif
#endif

long intervallo = 1000;

long templast = 0;
long lastPrintInstant = 0;

void setup(){

    pinMode(ALIVE_LED,OUTPUT);

}

void loop(){  

  if(millis()-templast > intervallo){
    digitalWrite(ALIVE_LED,!digitalRead(ALIVE_LED));
    templast = millis(); 
  } 


}
