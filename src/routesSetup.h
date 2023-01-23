
// #include <LittleFS.h>
// #include <ArduinoJson.h>


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


bool routesSetup(){


  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.on("/JSONrooms", HTTP_GET, [](AsyncWebServerRequest *request) {
    // send rooms.json as response
    request->send(LittleFS, "/rooms.json", "application/json");
  });

  server.on("/JSONdevices", HTTP_GET, [](AsyncWebServerRequest *request) {
    // send actual devices object as Json in response

    Serial.println("Sending devices data");
    String jsonStr;
    serializeJson(devices, jsonStr);
    request->send(200, "application/json", jsonStr);
  });

 server.on("/updateRoom", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total) {
        for (size_t i = 0; i < len; i++) {
          Serial.write(data[i]);
        }
        // Parse json from request to jsonData
        String jsonStr = (char *)data;
        DynamicJsonDocument jsonData(2024);
        DeserializationError error = deserializeJson(jsonData, jsonStr);
        
        Serial.println("updateRoom::::::");
        Serial.println(jsonStr);
      
        // if (error) {
        //   request->send(400, "application/json", "{\"error\":\"invalidJson\"}");
        //   return;
        // }
  
        // // read rooms.json
        // File rooms = LittleFS.open("/rooms.json", "r");
        // DynamicJsonDocument roomsData(2024);
        // DeserializationError error2 = deserializeJson(roomsData, rooms);
        // if (error2) {
        //   request->send(400, "application/json", "{\"error\":\"errorReadingJsonfromDisk\"}");
        //   return;
        // }
        // // find room in rooms.json
        // String roomName = jsonData["roomName"]; 
        // JsonArray roomsArray = roomsData["rooms"].as<JsonArray>();


        // // find room id in rooms.json
        // int roomId = -1;



        // update room  
        // save rooms to file



        // if (saveFile("/rooms.json", jsonStr)) {
        //   request->send(200, "application/json","{\"response\":\"dataSaved\"}");
        // } else {
        //   request->send(500, "application/json", "{\"error\":\"saveFailed\"}");
        // }
         });

  server.on(
      "/saveRooms", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,
      [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total) {
        for (size_t i = 0; i < len; i++) {
          Serial.write(data[i]);
        }

        String jsonStr = (char *)data;

        DynamicJsonDocument jsonData(2024);
        DeserializationError error = deserializeJson(jsonData, jsonStr);
        if (error) {
          request->send(400, "application/json", "{\"error\":\"invalidJson\"}");
          return;
        }
        if (saveFile("/rooms.json", jsonStr)) {
          request->send(200, "application/json",
                        "{\"response\":\"dataSaved\"}");
        } else {
          request->send(500, "application/json", "{\"error\":\"saveFailed\"}");
        }
      });

  server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/rooms.html", "text/html");
  });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(LittleFS, "/index.html", "text/html");
  });

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
  server.serveStatic("/_app/", LittleFS, "/_app/");
  server.serveStatic("/_app/immutable/", LittleFS, "/_app/immutable/");
  server.serveStatic("/_app/immutable/assets/", LittleFS, "/_app/immutable/assets/");
  server.serveStatic("/_app/immutable/chunks/", LittleFS, "/_app/immutable/chunks/");
  server.serveStatic("/_app/immutable/components/", LittleFS, "/_app/immutable/components/");
  server.serveStatic("/_app/immutable/components/pages/", LittleFS, "/_app/immutable/components/pages/");
  server.serveStatic("/_app/immutable/modules/", LittleFS, "/_app/immutable/modules/");


return true;

};

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len) {
  if(type == WS_EVT_CONNECT){
    // kod wykonywany po nawiązaniu połączenia
    client->printf("Hello Client %u", client->id());
    client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    // kod wykonywany po rozłączeniu
  } else if(type == WS_EVT_DATA){
    // kod obsługujący dane przesłane przez klienta
   // get WS data as JSON object


  }
}


void initWS(){

   // kod inicjalizujący Websocket
  server.addHandler(&ws);
  ws.onEvent(onWsEvent);



}

