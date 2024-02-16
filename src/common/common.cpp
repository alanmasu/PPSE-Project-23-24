#include <Arduino.h>
#include <Stream.h>
#include "common.h"

#ifdef ESP8266
    #include <ESP8266WiFi.h>
#else
    #include <WiFi.h>
#endif

void MsgData::write(Stream& stream) {
    uint16_t size = sizeof(MsgData) - sizeof(ssid) + (ssid.length() + 1) * 8;
    stream.write(SOH);
    stream.write((uint8_t*)&size, sizeof(size));
    stream.write(STX);
    stream.write((uint8_t*)&ledState, sizeof(ledState));
    stream.write((uint8_t*)&ledCount, sizeof(ledCount));
    stream.write((uint8_t*)&ledColor, sizeof(ledColor));
    stream.write((uint8_t*)&ledBrightness, sizeof(ledBrightness));
    stream.write((uint8_t*)&connState, sizeof(connState));
    for(uint8_t i = 0; i < 3; i++) {
      stream.write((uint8_t*)&ip[i], sizeof(ip[i]));
    }
    stream.write((uint8_t*)&apMode, sizeof(apMode));
    stream.write(GS);
    stream.print(ssid);
    stream.write(EOT);
}

void MsgData::read(Stream& stream) {
    uint16_t size;
    while(stream.read() != SOH);
    while(stream.read() != STX);
    stream.readBytes((uint8_t*)&size, sizeof(size));
    stream.readBytes((uint8_t*)&ledState, sizeof(ledState));
    stream.readBytes((uint8_t*)&ledCount, sizeof(ledCount));
    stream.readBytes((uint8_t*)&ledColor, sizeof(ledColor));
    stream.readBytes((uint8_t*)&ledBrightness, sizeof(ledBrightness));
    stream.readBytes((uint8_t*)&connState, sizeof(connState));
    for(uint8_t i = 0; i < 3; i++) {
      stream.readBytes((uint8_t*)&ip[i], sizeof(ip[i]));
    }
    stream.readBytes((uint8_t*)&apMode, sizeof(apMode));
    char c;
    stream.readBytes((uint8_t*)&c, sizeof(c));
    ssid = "";
    while(c != EOT) {
      ssid += c;
      stream.readBytes((uint8_t*)&c, sizeof(c));
    }
}
 

String splitString(String str, char sep, int index) {
  /* str a' la variabile di tipo String che contiene il valore da splittare
     sep a' ia variabile di tipo char che contiene il separatore (bisoga usare l'apostrofo: splitString(xx, 'xxx', yy)
     index a' la variabile di tipo int che contiene il campo splittato: str = "11111:22222:33333" se index= 0;
                                                                       la funzione restituira': "11111"
  */
  int found = 0;
  int strIdx[] = { 0, -1 };
  int maxIdx = str.length() - 1;

  for (int i = 0; i <= maxIdx && found <= index; i++) {
    if (str.charAt(i) == sep || i == maxIdx) {
      found++;
      strIdx[0] = strIdx[1] + 1;
      strIdx[1] = (i == maxIdx) ? i + 1 : i;
    }
  }
  return found > index ? str.substring(strIdx[0], strIdx[1]) : "";
}