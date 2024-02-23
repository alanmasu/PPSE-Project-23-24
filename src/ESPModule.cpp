#include <Arduino.h>
#include <ESPModule.h>

ESPModule::ESPModule(int baudrate, SerialUART& serialToUse, pin_size_t txPin, pin_size_t rxPin, pin_size_t enable){
    this->serialToUse = serialToUse;
    this->baudrate = baudrate;
    this->txPin = txPin;
    this->rxPin = rxPin;
    this->enable = enable;
}

void ESPModule::getData(WiFiConfiguration_t& wifiConfig){
    String str;
    while(serialToUse.available()>0) {
        str = serialToUse.readStringUntil('\n');
    }
    if(str != ""){
        Serial.print("Data recived: " + str + "\n");
        memcpy(wifiConfig.ssid, splitString(str, ';', 0).c_str(), 32);
        wifiConfig.ssid[32] = '\0';
        String ip = splitString(str, ';', 1);
        wifiConfig.ipAddress.fromString(ip);
        wifiConfig.ap = splitString(str, ';', 2).toInt();
        memcpy(wifiConfig.commitHash, splitString(str, ';', 3).c_str(), 8);
    }
}

void ESPModule::begin(){
    this->serialToUse.setTX(txPin);
    this->serialToUse.setRX(rxPin);
    this->serialToUse.begin(baudrate);
    this->serialToUse.begin(this->baudrate);
    pinMode(this->enable, OUTPUT);
    digitalWrite(this->enable, HIGH);
}

void ESPModule::sendData(const ApplicationRecord_t* applicationRecord){
    this->serialToUse.write((uint8_t*)applicationRecord, sizeof(applicationRecord));
}