
// #include <LittleFS.h>
// #include <ArduinoJson.h>


AsyncWebServer server(80);
AsyncWebSocket ws("/ws");


bool routesSetup(){


  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");

  server.on("/JSONrooms", HTTP_GET, [](AsyncWebServerRequest *request) {
    // send rooms.json as response
    request->send(LittleFS, "/assets/rooms.json", "application/json");
  });

  server.on("/JSONdevices", HTTP_GET, [](AsyncWebServerRequest *request) {
    // send actual devices object as Json in response

    Serial.println("Sending devices data");
    String jsonStr;
    serializeJson(devices, jsonStr);
    request->send(200, "application/json", jsonStr);
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
        if (saveFile("/assets/rooms.json", jsonStr)) {
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
  server.serveStatic("/assets/", LittleFS, "/assets/");
  server.serveStatic("/assets/img/", LittleFS, "/assets/img/");

return true;

};
