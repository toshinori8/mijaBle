//////////////////// LIBS ////////////////////  
#include "ESPConnect.h"
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <AsyncWebSocket.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
//////////////////////////////////PROGRAM LOGIC///
#include <ble.h>
#include <ota.h>
#include <time.h>
#include <Timers.h>
#include <routesSetup.h>
//////////////////////////////////////////


bool littleFSinitialised = false;


void setup() {
  
  Serial.begin(115200);

  if (ESPConnect.begin(&server)) {
    Serial.println("Connected to WiFi");
    Serial.println("IPAddress: " + WiFi.localIP().toString());

  } else {
    Serial.println("Failed to connect to WiFi");
  }

  initTime();
   if (MDNS.begin(hostName)) {

    // CORS VALIDATION DISABLE
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "content-type");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");


  // DefaultHeaders::Instance().addHeader("Access-Control-Expose-Headers", "content-type");
  // DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "86400");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Credentials", "true");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "X-Requested-With, X-HTTP-Method-Override, Content-Type, Accept");
  // DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  // DefaultHeaders::Instance().addHeader("Access-Control-Request-Headers", "content-type");
  // DefaultHeaders::Instance().addHeader("Access-Control-Request-Method", "POST");
  // DefaultHeaders::Instance().addHeader("Allow", "GET, POST, HEAD, OPTIONS");
 
  
  // CORS VALIDATION ENABLE




#ifdef CORS_DEBUG
    DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
    DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
#endif


    server.begin();
    
    Serial.println("ASYNC server started");
    Serial.print("MDNS responder started at: http://");
    Serial.print(hostName);
    Serial.print(".local at: ");
    Serial.println(WiFi.localIP());
  }
  otaStart(hostName);

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    
    Serial.println("Formatting LittleFS");
    if (!LittleFS.format()) {
        Serial.println("LittleFS format failed");
    return;
    }else{
      Serial.println("LittleFS formatted: please upload files again & restart device");
    }
    return;
  }else{


  // set routes of webserver
  routesSetup();
  // initWS();
  // loadDevices();
  // initBluetooth();

    // read contetnt of /rooms.json
    rooms = LittleFS.open("/robots.txt", "r");
    if (!rooms) {
      Serial.println("Failed to open file for reading");
    } else {
      Serial.println("File opened for reading");
      while (rooms.available()) {

       
        Serial.write(rooms.read());
      }

 littleFSinitialised = true; 
 // set routes of webserver
  routesSetup();
  // initWS();
  loadDevices();
  initBluetooth();



      rooms.close();
    }

  }

 

}

void loop() {
  ArduinoOTA.handle();
  if(!littleFSinitialised){return;} 
  ble_scan_loop();
  // timers.process();
  // sleepTrigger();
}
