#ifndef __ESP_MODULE_H__
#define __ESP_MODULE_H__

#include <Arduino.h>
#include <Stream.h>
#include <IPAddress.h>
#include <common.h>

class ESPModule {
    public:
        ESPModule(SerialUART& serialToUse = Serial1, int baudrate = 115200, uint8_t txPin = 4, uint8_t rxPin = 5);
        inline void begin(){
            serialToUse.begin(this->baudrate);
        }
        WiFiConfiguration_t getData();
        void sendData(const ApplicationRecord_t& data);
    private:
        SerialUART& serialToUse;
        int baudrate;
};




#endif
