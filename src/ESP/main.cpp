#include <esp-fs-webserver.h>  // https://github.com/cotestatnt/esp-fs-webserver

#include <FS.h>
#include <LittleFS.h>
#include <git_revision.h>
#include "common.h"

#define FILESYSTEM LittleFS

#ifndef LED_BUILTIN
  #define LED_BUILTIN 2
#elif defined(ESP8266) && defined(LED_BUILTIN)
  #undef LED_BUILTIN
  #define LED_BUILTIN 2
#endif
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

ApplicationRecord_t applicationRecord;

WiFiConfiguration_t wifiConfig;

uint64_t t0 = 0;

bool btnStatus = false;
bool btnStatusP = false;

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
  if(webRequest->hasArg("val")) {
    int value = webRequest->arg("val").toInt();
    #ifdef ESP32
      digitalWrite(ledPin, value);
    #else
      #if defined(ESP8266)
        digitalWrite(ledPin, !value);
      #endif
    #endif
  }

  String reply = "LED is now ";
  reply += digitalRead(ledPin) ? "OFF" : "ON";
  webRequest->send(200, "text/plain", reply);
}

void handleMainEndpoint() {
  WebServerClass* webRequest = myWebServer.getRequest();
  String reply = "{\"cpuTemp\": \"" + String(applicationRecord.cpuTemp) + "\",\"temp\":\""+ String(applicationRecord.temp) + "\"}";
  webRequest->send(200, "text/json", reply);
}

void handleActualPosition(){
  StaticJsonDocument<200> doc;
  WebServerClass* webRequest = myWebServer.getRequest();
  
  doc["time"] = mktime(&applicationRecord.actualPoint.timeInfo);
  JsonArray actualCoordinates = doc.createNestedArray("actualCoordinates");
  actualCoordinates[0] = applicationRecord.actualPoint.latitude;
  actualCoordinates[1] = applicationRecord.actualPoint.longitude;
  doc["fix"] = applicationRecord.actualPoint.fix;
  doc["fixType"] = applicationRecord.actualPoint.fixType;
  doc["hdop"] = applicationRecord.actualPoint.hdop;
  doc["sats"] = applicationRecord.actualPoint.sats;
  doc["temp"] = applicationRecord.temp;

  String reply;
  serializeJson(doc, reply);
  webRequest->send(200, "text/json", reply);
}

void handleWaypointList(){
  StaticJsonDocument<200> doc;
  WebServerClass* webRequest = myWebServer.getRequest();
  
  JsonArray waypoints = doc.to<JsonArray>();
  JsonObject obj = waypoints.createNestedObject();
  obj["time"] = mktime(&applicationRecord.firstWayPoint.timeInfo);
  JsonArray actualCoordinates = obj.createNestedArray("actualCoordinates");
  actualCoordinates[0] = applicationRecord.firstWayPoint.latitude;
  actualCoordinates[1] = applicationRecord.firstWayPoint.longitude;
  obj["fix"] = applicationRecord.firstWayPoint.fix;
  obj["fixType"] = applicationRecord.firstWayPoint.fixType;
  obj["hdop"] = applicationRecord.firstWayPoint.hdop;
  obj["temp"] = applicationRecord.firstWayPoint.temp;
  obj["sats"] = applicationRecord.firstWayPoint.sats;
  obj["waypointAdded"] = applicationRecord.waypointsaved;


  String reply;
  serializeJson(doc, reply);
  webRequest->send(200, "text/json", reply);
}


void setup(){
  SERIAL_TO_USE.begin(115200);

  pinMode(LED_BUILTIN, OUTPUT);
  #ifdef ESP32
    pinMode(0, INPUT);
    btnStatus = !digitalRead(0);
    btnStatusP = btnStatus;    
  #endif

  #ifdef ESP8266
    digitalWrite(LED_BUILTIN, HIGH);
    ESP.wdtEnable(3000);
  #endif
  #ifdef ESP32
    Serial.begin(115200);
    Serial.printf("Git info: %s %s\n", __GIT_COMMIT__, __GIT_REMOTE_URL__);
    Serial.printf("Built on %s at %s\n", __DATE__, __TIME__);
    Serial.printf("ApplictionRecord_t size: %d\n", sizeof(ApplicationRecord_t));
  #else
    SERIAL_TO_USE.printf("Git info: %s %s\n", __GIT_COMMIT__, __GIT_REMOTE_URL__);
    SERIAL_TO_USE.printf("Built on %s at %s\n", __DATE__, __TIME__);
  #endif

  //Copio il commit hash per versioning
  memcpy(wifiConfig.commitHash, __GIT_COMMIT__, 8);
  wifiConfig.commitHash[8] = '\0';

  // FILESYSTEM INIT
  startFilesystem();

  // Try to connect to flash stored SSID, start AP if fails after timeout
  myWebServer.setAP("PPSE-23_AP", "123456789");

  #ifdef ESP8266
    IPAddress myIP = myWebServer.startWiFi(15000, true, [](){
        ESP.wdtFeed();
    } );
  #else
    IPAddress myIP = myWebServer.startWiFi(15000, true);
  #endif

  // Add custom page handlers to webserver
  myWebServer.addHandler("/mainPageEndpoint", HTTP_GET, handleMainEndpoint);
  myWebServer.addHandler("/actualPosition", HTTP_GET, handleActualPosition);
  myWebServer.addHandler("/waypointList", HTTP_GET, handleWaypointList);
  myWebServer.addHandler("/led", HTTP_GET, handleLed);

  // Start webserver
  if (myWebServer.begin()) {
    SERIAL_TO_USE.print(F("ESP Web Server started on IP Address: "));
    SERIAL_TO_USE.println(myIP);
    SERIAL_TO_USE.println(F("Open /setup page to configure optional parameters"));
    SERIAL_TO_USE.println(F("Open /edit page to view and edit files"));
    SERIAL_TO_USE.println(F("Open /update page to upload firmware and filesystem updates"));
  }
}

void loop() {
  size_t bytesRead = 0;
  if(SERIAL_TO_USE.available() > 0){
    bytesRead = SERIAL_TO_USE.readBytes((char*)&applicationRecord, sizeof(applicationRecord));
  }
  #ifdef ESP32
    //Reading button on GPIO0
    btnStatus = !digitalRead(0);
    if(btnStatus != btnStatusP && btnStatus  || (applicationRecord.gotoAP && !myWebServer.getAPMode()) ){
      myWebServer.startAP();
      Serial.println("Starting AP");
    }
    btnStatusP = btnStatus;
  #else
    ESP.wdtFeed();
    if(applicationRecord.gotoAP && !myWebServer.getAPMode()){
      myWebServer.startAP();
      Serial.println("Starting AP");
    }
  #endif


  myWebServer.run();
  if(millis() - t0 > 1000){
    t0 = millis();
    // Serial.printf("Packet recived:\n\ttemp: %f,\n\tcpuTemp: %f,\n\tgotoAP: %d", applicationRecord.temp, applicationRecord.cpuTemp, applicationRecord.gotoAP);
    wifiConfig.ap = myWebServer.getAPMode();
    if(!wifiConfig.ap){
      if(WiFi.status() == WL_CONNECTED){
        wifiConfig.ipAddress = WiFi.localIP();
        memcpy(wifiConfig.ssid, WiFi.SSID().c_str(), 32);
      }else{
        wifiConfig.ipAddress = IPAddress();
        strcpy(wifiConfig.ssid, "Not connected!");
      }
    }else{
      wifiConfig.ipAddress = WiFi.softAPIP();
      memcpy(wifiConfig.ssid, WiFi.softAPSSID().c_str(), 32);
    }
    wifiConfig.ssid[32] = '\0';
    
    // SERIAL_TO_USE.write((uint8_t*)&wifiConfig, sizeof(wifiConfig));
    SERIAL_TO_USE.printf("%s;", wifiConfig.ssid);
    SERIAL_TO_USE.print(wifiConfig.ipAddress);
    SERIAL_TO_USE.printf(";%d;%s\n", wifiConfig.ap, wifiConfig.commitHash);
    #ifdef ESP32
      Serial.printf("SSID: %s, IP: %d.%d.%d.%d, AP: %d, commit: %s\n",  wifiConfig.ssid, 
                                                                                          wifiConfig.ipAddress[0], 
                                                                                          wifiConfig.ipAddress[1], 
                                                                                          wifiConfig.ipAddress[2], 
                                                                                          wifiConfig.ipAddress[3], 
                                                                                          wifiConfig.ap,
                                                                                          wifiConfig.commitHash);
    #endif
  }
}