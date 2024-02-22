#ifndef __ESP_MODULE_H__
#define __ESP_MODULE_H__

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>
#include <common.h>

class ESPModule {
    public:
        ESPModule(SerialUART& serialToUse, int baudrate = 115200);
        void begin(uint8_t txPin = 4, uint8_t rxPin = 5);
        WiFiConfiguration_t getData();
        void sendData(const ApplicationRecord_t& data);
    private:
        SerialUART& serialToUse;
        int baudrate;
};




#endif
