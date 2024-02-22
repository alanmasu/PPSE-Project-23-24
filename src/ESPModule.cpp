#include <Arduino.h>
#include <ESPModule.h>

ESPModule::ESPModule(SerialUART& serialToUse, int baudrate) : 
serialToUse(serialToUse), 
baudrate(baudrate){
}

WiFiConfiguration_t ESPModule::getData(){
    WiFiConfiguration_t wifiConfig;
    String str;
    while(serialToUse.available()>0) {
        str = serialToUse.readStringUntil('\n');
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

void ESPModule::begin(uint8_t txPin, uint8_t rxPin){
    serialToUse.setTX(txPin);
    serialToUse.setRX(rxPin);
    serialToUse.begin(this->baudrate);
}

void ESPModule::sendData(const ApplicationRecord_t& applicationRecord){
    serialToUse.write((byte*)&applicationRecord, sizeof(applicationRecord));
}