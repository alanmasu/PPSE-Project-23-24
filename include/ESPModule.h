#ifndef __ESP_MODULE_H__
#define __ESP_MODULE_H__

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>
#include <common.h>

class ESPModule {
    public:
        ESPModule(int baudrate = 115200, pin_size_t txPin = 4, pin_size_t rxPin = 5);
        void begin(SerialUART* serialToUse);
        void getData(WiFiConfiguration_t& wifiConfig);
        void sendData(const ApplicationRecord_t* data);
    private:
        SerialUART* serialToUse;
        int baudrate;
        pin_size_t txPin;
        pin_size_t rxPin;
};




#endif
