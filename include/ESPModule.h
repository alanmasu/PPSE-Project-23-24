#ifndef __ESP_MODULE_H__
#define __ESP_MODULE_H__

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>
#include <common.h>

class ESPModule {
    public:
        ESPModule(int baudrate = 115200, SerialUART& serialToUse = Serial1, pin_size_t txPin = 4, pin_size_t rxPin = 5, pin_size_t enable = 23);
        void begin();
        void getData(WiFiConfiguration_t& wifiConfig);
        void sendData(const ApplicationRecord_t* data);
    private:
        SerialUART& serialToUse = Serial1;
        int baudrate;
        pin_size_t txPin;
        pin_size_t rxPin;
        pin_size_t enable;
        size_t bufferQueue;
};




#endif
