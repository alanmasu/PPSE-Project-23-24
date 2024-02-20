#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(2000);
  Serial.println("WS2812 Test code started...");
  Serial.print("CPU Clock Frequency: ");
  Serial.print(rp2040.f_cpu() / 1000000.0);
  Serial.println(" MHz");
}

void loop() {
  delay(200);
}