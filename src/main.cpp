#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>
#include <WiFiClientSecure.h>
#include <AsyncWebSocket.h>
#include "AsyncJson.h"
#include <ESPmDNS.h>
#include <sstream>
#include "time.h"
#include <ble.h>
#include <ota.h>

char * hostName = "CleargrassTermostat";

int SLEEP_TIME = 10; //seconds

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// define files
File rooms;

 
void ble_scan_loop();

// function to save data to file
bool saveFile(String path, String data){
 File file = LittleFS.open(path, "w");
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  } else{
      file.print(data);
      Serial.print("File saved: ");
      file.close();
      return true;
  }
};

void setup() 
{
  Serial.begin(115200);
  
  // Load devices data stored with names;
  
 

  // Connect to Wi-Fi network with SSID and password
  WiFi.begin("oooooio", "pmgana921");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connected to WiFi..");
  }

  // WebSocket server started
  Serial.println("WebSocket server started");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Current device time: " + getTime());
  
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.on("/JSONrooms", HTTP_GET, [](AsyncWebServerRequest *request){
    // send rooms.json as response 
    request->send(LittleFS, "/assets/rooms.json", "application/json");
  });

  server.on("/JSONdevices", HTTP_GET, [](AsyncWebServerRequest *request){
    
    // send actual devices object as Json in response

    Serial.println("Sending devices data");
    String jsonStr;      
    serializeJson(devices, jsonStr);
    request->send(200, "application/json", jsonStr);

  });

// Save devices data from request 
//   AsyncCallbackJsonWebHandler *handlerSaveDevices = new AsyncCallbackJsonWebHandler("/saveDevices", [](AsyncWebServerRequest *request, JsonVariant &json) {

//   StaticJsonDocument<2024> data;
//   if (json.is<JsonArray>())
//   {
//     data = json.as<JsonArray>();
//   }
//   else if (json.is<JsonObject>())
//   {
//     data = json.as<JsonObject>();
//   } 

         
//     String jsonStr;      
//     serializeJson(data, jsonStr);
//     if(saveFile("/assets/devices.json", jsonStr )){
              

//     request->send(200, "application/json", "{\"response\":\"dataSaved\"}", 
//         { "Access-Control-Allow-Origin": "*" });

//   } else{
//     Serial.println("Error saving file");
//     request->send(200, "application/json", "{\"response\":\"errorSavingData\"}");
//   };

 
// });


//   AsyncCallbackJsonWebHandler *handlerSaveRooms = new AsyncCallbackJsonWebHandler("/saveRooms", [](AsyncWebServerRequest *request, JsonVariant &json) {

//   StaticJsonDocument<2024> data;
//   if (json.is<JsonArray>())
//   {
//     data = json.as<JsonArray>();
//   }
//   else if (json.is<JsonObject>())
//   {
//     data = json.as<JsonObject>();
//   } 
          
//     String jsonStr;      
//     serializeJson(data, jsonStr);
//     if(saveFile("/assets/rooms.json", jsonStr )){

//     request->send(200, "application/json", "{\"response\":\"dataSaved\"}");
//   } else{
//     Serial.println("Error saving file");
//     request->send(200, "application/json", "{\"response\":\"errorSavingData\"}");
//   };

 
// });

  // server.addHandler(handlerSaveRooms);
  // server.addHandler(handlerSaveDevices);
  

server.on("/saveRooms", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL,[](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 

    for (size_t i = 0; i < len; i++) {
        Serial.write(data[i]);
    }
 
       
    String jsonStr = (char*)data;
    
    DynamicJsonDocument jsonData(2024);
    DeserializationError error = deserializeJson(jsonData, jsonStr);
    if (error) {
        request->send(400, "application/json", "{\"error\":\"invalidJson\"}");
        return;
    }
    if(saveFile("/assets/rooms.json", jsonStr )){
        request->send(200, "application/json", "{\"response\":\"dataSaved\"}");
    }else{
        request->send(500, "application/json", "{\"error\":\"saveFailed\"}");
    }

 
      
  });  



  /// SERVE ROOMS PAGE 
  server.on("/assets/rooms.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/rooms.css", "text/css");
  });
  server.on("/assets/rooms.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/rooms.js", "text/javascript");
  });
  server.on("/rooms.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/rooms.html", "text/html");
  });
  server.on("/rooms", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/rooms.html", "text/html");
  });


  /// SERVE INDEX PAGE 
  server.on("/assets/index.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/index.css", "text/css");
  });
  server.on("/assets/index.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/index.js", "text/javascript");
  });
  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/index.html", "text/html");
  });
  server.on("aclock.ttf", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/assets/aclock.ttf", "font/ttf");
  }); 





if (MDNS.begin(hostName)) {
    server.begin();
    Serial.println("ASYNC server started");
    Serial.print("MDNS responder started at: http://");
    Serial.print(hostName);
    Serial.print(".local at: ");
    Serial.println(WiFi.localIP());
  }
  // HTTP server started
  

  //bluetooth
   loadDevices();
  initBluetooth();

  otaStart(hostName);
}

void loop()
{
      ArduinoOTA.handle();
      ble_scan_loop();
      //timers.process();

#if SLEEP_TIME > 0
  //esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000); // translate second to micro second
  Serial.printf("Enter deep sleep for %d seconds...\n\r", (SLEEP_TIME));
  delay(10);
  //esp_deep_sleep_start();
  esp_deep_sleep(1000000LL * SLEEP_TIME);
  Serial.println(F("After deep sleep"));
#endif
}

