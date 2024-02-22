#include <Arduino.h>
#include <ESPModule.h>

ESPModule::ESPModule(SerialUART & serialToUse, int baudrate, uint8_t txPin, uint8_t rxPin) : 
serialToUse(serialToUse), 
baudrate(baudrate){
    serialToUse.setTX(txPin);
    serialToUse.setRX(rxPin);
}

WiFiConfiguration_t ESPModule::getData(){
    WiFiConfiguration_t wifiConfig;
    String str;
    while(Serial1.available()>0) {
        str = Serial1.readStringUntil('\n');
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

void ESPModule::sendData(const ApplicationRecord_t& applicationRecord){
    serialToUse.write((byte*)&applicationRecord, sizeof(applicationRecord));
}