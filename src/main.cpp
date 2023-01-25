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

// init timers
Timers<4> timers;

bool littleFSinitialised = false;

// attach clas define termostatRules





void setup()
{
  Serial.begin(115200);

  if (ESPConnect.begin(&server))
  {
    Serial.println(F("Connected to WiFi"));
    Serial.println("IPAddress: " + WiFi.localIP().toString());
  }
  else
  {
    Serial.println(F("Failed to connect to WiFi"));
  }

  initTime();
  if (MDNS.begin(hostName))
  {
    // CORS VALIDATION DISABLE
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
    // CORS VALIDATION ENABLE

// #ifdef CORS_DEBUG
//     DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), F("*"));
//     DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), F("content-type"));
// #endif

    server.begin();

    Serial.println(F("ASYNC server started"));
    Serial.print(F("MDNS responder started at: http://"));
    Serial.print(hostName);
    Serial.print(F(".local at: "));
    Serial.println(WiFi.localIP());
  }
  otaStart(hostName);

  if (!LittleFS.begin())
  {
    Serial.println(F("An Error has occurred while mounting LittleFS"));

    Serial.println(F("Formatting LittleFS"));
    if (!LittleFS.format())
    {
      Serial.println(F("LittleFS format failed"));
      return;
    }
    else
    {
      Serial.println(F("LittleFS formatted: please upload files again & restart device"));
    }
    return;
  }
  else
  {
    // set routes of webserver
    routesSetup();
    // initWS();
    // loadDevices();
    // initBluetooth();
    // read contetnt of /rooms.json
    roomsF = LittleFS.open("/rooms.json", "r");
    if (!roomsF)
    {
      Serial.println(F("Failed to open file for reading"));
    }
    else
    {
      Serial.println(F("File opened for reading"));
      while (roomsF.available())
      {
        Serial.write(roomsF.read());
      }
      littleFSinitialised = true;
      // set routes of webserver
      routesSetup();
      // initWS();
      loadDevicesToMemory();
      initBluetooth();
    }
  }
  // init timers
  // timers.attach(0, 1000, function01);
}

void loop()
{
  ArduinoOTA.handle();
  if (!littleFSinitialised)
  {
    return;
  }
  ble_scan_loop();
  timers.process();
  // sleepTrigger();
}
