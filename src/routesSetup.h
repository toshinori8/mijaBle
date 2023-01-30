bool routesSetup()
{

  // on not found send index.html
  // server.onNotFound([](AsyncWebServerRequest *request)
  //                 {
  //                   if (request->method() == HTTP_OPTIONS)
  //                   {
  //                     request->send(200);
  //                   }
  //                   else
  //                   {
  //                     request->send(404, "application/json", "{\"message\":\"Not found\"}");
  //                   }
  //                 });

  server.on("/data/JSONrooms", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    request->send(LittleFS, "/rooms.json", "application/json"); 
    Serial.println(F("Sending rooms data"));
  });

  server.on("/data/JSONdevices", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    // send actual devices object as Json in response
    Serial.println(F("Sending devices data"));
    String jsonStr;
    serializeJson(devices, jsonStr);
    request->send(200, "application/json", jsonStr);
    jsonStr.clear();

     });

  server.on("/data/updateRoom", HTTP_OPTIONS, [](AsyncWebServerRequest *request)
            { request->send(204);
            });

  server.on("/data/updateRoom", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total)
      {
        // for (size_t i = 0; i < len; i++)
        // {
        //  // Serial.write(data[i]);
        // }

        if (!data)
        {
          request->send(400, "application/json", "{\"error\":\"invalidData\"}");
          Serial.print(F("[  invalidData  ]"));
          return;
        }
        if (data)
        {
          // parse data from request
          String jsonStr = (char *)data;
            
          DynamicJsonDocument jsonData(1024);
          DeserializationError error = deserializeJson(jsonData, jsonStr);
          if (error)
          {
            request->send(400, "application/json", "{\"error\":\"invalidJson\"}");
            Serial.print(F("[  invalidJson  ]"));
            return;
          }
          String id = jsonData["id"];

          roomsF = LittleFS.open("/rooms.json", "r");
          String roomsString = roomsF.readString(); // read file to string
          roomsF.close();
        
          DynamicJsonDocument roomsData(1024);
          DeserializationError error2 = deserializeJson(roomsData, roomsString);
          if (error2)
          {
            request->send(400, "application/json", "{\"error\":\"invalidJsonOnDevice\"}");
            return;
          }else{
              //Find room with ID in rommsData  
              for (int i = 0; i < roomsData.size(); i++)
              {
                
                
                if (roomsData[i]["id"] == id)
                {

                  Serial.println("Updating room: [ " + id+" ]");
                  roomsData[i]["name"] = jsonData["name"];       
                  roomsData[i]["minTemp"] = jsonData["minTemp"]; 
                  roomsData[i]["mac"] = jsonData["mac"];          
                  
                  // save updated rooms file
                  String jsonStr;
                  serializeJson(roomsData, jsonStr);
                  saveFile("/rooms.json", jsonStr);
                  request->send(200, "application/json","{\"response\":\"dataSaved\"}");
                }
              }

          }
          
        request->send(200, "application/json","{\"response\":\"dataNotSaved\"}");  
        }
        else
        {
          request->send(500, "application/json", "{\"error\":\"saveFailed\"}");
        }
      });

  server.on("/data/saveRooms", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL,[](AsyncWebServerRequest *request, uint8_t *data, size_t len,
         size_t index, size_t total)
      {
        for (size_t i = 0; i < len; i++)
        {
          Serial.write(data[i]);
        }

        String jsonStr = (char *)data;

        DynamicJsonDocument jsonData(2024);
        DeserializationError error = deserializeJson(jsonData, jsonStr);
        if (error)
        {
          request->send(400, "application/json", "{\"error\":\"invalidJson\"}");
          return;
        }
        if (saveFile("/rooms.json", jsonStr))
        {
          request->send(200, "application/json",
                        "{\"response\":\"dataSaved\"}");
        }
        else
        {
          request->send(500, "application/json", "{\"error\":\"saveFailed\"}");
        }
      });

  server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/rooms.html", "text/html"); });

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(LittleFS, "/index.html", "text/html"); });


  server.on("/data/click", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              termostat.processData();
              request->send(200, "text/plain", "Hello, GET: " + request->url());
            });

  server.on("/aclock.ttf", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(LittleFS, "/aclock.ttf", "application/octet-stream");

            });
  server.on("/aclock.woff", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(LittleFS, "/aclock.woff", "application/octet-stream");

            });

  server.serveStatic("/", LittleFS, "/");
  server.serveStatic("/_app/", LittleFS, "/_app/");
  server.serveStatic("/_app/immutable/", LittleFS, "/_app/immutable/");
  server.serveStatic("/_app/immutable/assets/", LittleFS, "/_app/immutable/assets/");
  server.serveStatic("/_app/immutable/chunks/", LittleFS, "/_app/immutable/chunks/");
  server.serveStatic("/_app/immutable/components/", LittleFS, "/_app/immutable/components/");
  server.serveStatic("/_app/immutable/components/pages/", LittleFS, "/_app/immutable/components/pages/");
  server.serveStatic("/_app/immutable/modules/", LittleFS, "/_app/immutable/modules/");

  return true;
};

void onWsEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    // kod wykonywany po nawiązaniu połączenia
    client->printf("Hello Client %u", client->id());
    // wsClients.add(client);
    client->ping();
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    printf("Client %u disconnected", client->id());

    // remove client from list
    // wsClients.remove(client);
    
    // kod wykonywany po rozłączeniu
  }
  else if (type == WS_EVT_DATA)
  {
    // kod obsługujący dane przesłane przez klienta
    // get WS data as JSON object
  }
}

void initWS()
{
  server.addHandler(&ws);
  ws.onEvent(onWsEvent);
}
