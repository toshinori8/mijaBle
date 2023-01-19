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


// void ble_scan_loop();
// bool initTime();
// bool routesSetup();
// void sleepTrigger();

void setup() {
  
  Serial.begin(115200);

  if (ESPConnect.begin(&server)) {
    Serial.println("Connected to WiFi");
    Serial.println("IPAddress: " + WiFi.localIP().toString());

  } else {
    Serial.println("Failed to connect to WiFi");
  }

  initTime();

  if (!LittleFS.begin()) {
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  // set routes of webserver
  routesSetup();

  if (MDNS.begin(hostName)) {
    server.begin();
    Serial.println("ASYNC server started");
    Serial.print("MDNS responder started at: http://");
    Serial.print(hostName);
    Serial.print(".local at: ");
    Serial.println(WiFi.localIP());
  }
  loadDevices();
  initBluetooth();
  otaStart(hostName);
}

void loop() {
  ArduinoOTA.handle();
  ble_scan_loop();
  // timers.process();
  sleepTrigger();
}
