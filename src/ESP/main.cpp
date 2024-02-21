#include <esp-fs-webserver.h>  // https://github.com/cotestatnt/esp-fs-webserver

#include <FS.h>
#include <LittleFS.h>
#include <git_revision.h>
#include "common.h"

#define FILESYSTEM LittleFS

#define LED_BUILTIN 2

// In order to set SSID and password open the /setup webserver page
// const char* ssid;
// const char* password;

uint8_t ledPin = LED_BUILTIN;
bool apMode = false;

#ifdef ESP8266
  ESP8266WebServer server(80);
  #define SERIAL_TO_USE Serial
#elif defined(ESP32)
  WebServer server(80);
  #define SERIAL_TO_USE Serial2
#endif

FSWebServer myWebServer(FILESYSTEM, server);

ServerData data;

MsgData msgData;

////////////////////////////////  Filesystem  /////////////////////////////////////////
void startFilesystem(){
  // FILESYSTEM INIT
  if ( FILESYSTEM.begin()){
    File root = FILESYSTEM.open("/", "r");
    File file = root.openNextFile();
    while (file){
      const char* fileName = file.name();
      size_t fileSize = file.size();
      SERIAL_TO_USE.printf("FS File: %s, size: %lu\n", fileName, (long unsigned)fileSize);
      file = root.openNextFile();
    }
    SERIAL_TO_USE.println();
  }
  else {
    SERIAL_TO_USE.println("ERROR on mounting filesystem. It will be formmatted!");
    FILESYSTEM.format();
    ESP.restart();
  }
}

////////////////////////////  HTTP Request Handlers  ////////////////////////////////////
void handleLed() {
  WebServerClass* webRequest = myWebServer.getRequest();

  // http://xxx.xxx.xxx.xxx/led?val=1
  if(webRequest->hasArg("val")) {
    int value = webRequest->arg("val").toInt();
    digitalWrite(ledPin, value);
  }

  String reply = "LED is now ";
  reply += digitalRead(ledPin) ? "OFF" : "ON";
  webRequest->send(200, "text/plain", reply);
}

void handleMainEndpoint() {
  WebServerClass* webRequest = myWebServer.getRequest();

  String reply = "{\"cpuTemp\": \"" + String(data.cpuTemp) + "\",\"temp\":\""+ String(data.temp) + "\"}";
  webRequest->send(200, "text/json", reply);
}

void handleRgbLed(){
  WebServerClass* webRequest = myWebServer.getRequest();
  if(webRequest->hasArg("val")) {
    int value = webRequest->arg("val").toInt();
    msgData.ledState = value;
  }

  String reply = "LEDS are now ";
  reply += !msgData.ledState ? "OFF" : "ON";
  webRequest->send(200, "text/plain", reply);
}

void handlAactualPosition(){
  WebServerClass* webRequest = myWebServer.getRequest();
  StaticJsonDocument<200> doc;
  
  String reply = "{\"cpuTemp\": \"" + String(data.cpuTemp) + "\",\"temp\":\""+ String(data.temp) + "\"}";
  webRequest->send(200, "text/json", reply);
}

void handleSetLeds(){
  WebServerClass* webRequest = myWebServer.getRequest();
  if(webRequest->hasArg("color")) {
    int color = webRequest->arg("color").toInt();
    msgData.ledColor = color;
  }
  if(webRequest->hasArg("count")) {
    int count = webRequest->arg("count").toInt();
    msgData.ledCount = count;
  }
  // webRequest->sendHeader("Location", String("/"), true);
  // webRequest->send(307, "text/plain", "Temporary Redirect"); 
  webRequest->send(200, "text/plain", "Ok"); 
}

void setup(){
  Serial.begin(115200);
  Serial.printf("Git info: %s %s\n", __GIT_COMMIT__, __GIT_REMOTE_URL__);
  Serial.printf("Built on %s at %s\n", __DATE__, __TIME__);
  SERIAL_TO_USE.begin(115200);
  SERIAL_TO_USE.println(sizeof(IPAddress));
  // FILESYSTEM INIT
  startFilesystem();

  // Try to connect to flash stored SSID, start AP if fails after timeout
  IPAddress myIP = myWebServer.startWiFi(15000, "ESP8266_AP", "123456789" );

  // Add custom page handlers to webserver
  myWebServer.addHandler("/mainPageEndpoint", HTTP_GET, handleMainEndpoint);
  myWebServer.addHandler("/led", HTTP_GET, handleLed);
  myWebServer.addHandler("/rgbLed", HTTP_GET, handleRgbLed);
  myWebServer.addHandler("/setLeds", HTTP_POST, handleSetLeds);

  // Start webserver
  if (myWebServer.begin()) {
    SERIAL_TO_USE.print(F("ESP Web Server started on IP Address: "));
    SERIAL_TO_USE.println(myIP);
    SERIAL_TO_USE.println(F("Open /setup page to configure optional parameters"));
    SERIAL_TO_USE.println(F("Open /edit page to view and edit files"));
    SERIAL_TO_USE.println(F("Open /update page to upload firmware and filesystem updates"));
  }

  pinMode(LED_BUILTIN, OUTPUT);

  //Initialize the WS2812 LED strip
  msgData.ledBrightness = 255;

}

uint64_t t0 = 0;

void loop() {
  if(SERIAL_TO_USE.available() > 0){
    SERIAL_TO_USE.readBytes((char*)&data, sizeof(ServerData)) != sizeof(ServerData);
    // if(SERIAL_TO_USE.readBytes((char*)&data, sizeof(ServerData)) != sizeof(ServerData)){
    //   SERIAL_TO_USE.println("ERROR: SERIAL_TO_USE.readBytes() failed!");
    // }else{
    //   SERIAL_TO_USE.printf("CPU Temp: %f, Temp: %f\n", data.cpuTemp, data.temp);
    // }
  }
  myWebServer.run();
  if(millis() - t0 > 1000){
    t0 = millis();
    SERIAL_TO_USE.printf("%d;%d;%d;%d;%d;", msgData.ledState, msgData.ledCount, msgData.ledColor, msgData.ledBrightness, msgData.connState);
    SERIAL_TO_USE.print(WiFi.localIP());
    SERIAL_TO_USE.printf(";%d;%s\n", msgData.apMode, WiFi.SSID().c_str());
  }
}