#include <Arduino.h>
#include <ESPModule.h>

ESPModule::ESPModule(int baudrate, pin_size_t txPin, pin_size_t rxPin){
    this->baudrate = baudrate;
    this->txPin = txPin;
    this->rxPin = rxPin;
}

WiFiConfiguration_t ESPModule::getData(){
    WiFiConfiguration_t wifiConfig;
    String str;
    while(serialToUse->available()>0) {
        str = serialToUse->readStringUntil('\n');
    }
    if(str != ""){
        memcpy(wifiConfig.ssid, splitString(str, ';', 0).c_str(), 32);
        wifiConfig.ssid[32] = '\0';
        String ip = splitString(str, ';', 1);
        // wifiConfig.ipAddress = IPAddress();
        wifiConfig.ipAddress.fromString(ip);
        wifiConfig.ap = splitString(str, ';', 2).toInt();
    }
    return wifiConfig;
}

void ESPModule::begin(SerialUART* serialToUse){
    this->serialToUse = serialToUse;
    serialToUse->setTX(txPin);
    serialToUse->setRX(rxPin);
    serialToUse->begin(baudrate);
    serialToUse->begin(this->baudrate);
}

void ESPModule::sendData(const ApplicationRecord_t* applicationRecord){
    this->serialToUse->write((uint8_t*)applicationRecord, sizeof(applicationRecord));
}