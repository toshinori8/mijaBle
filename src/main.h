#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <termostatClass.h>
#include <relayClass.h>

DynamicJsonDocument rooms(2048); // Json document for storing rooms data
DynamicJsonDocument devices(2048); // Json document for storing devices data

AsyncWebServer server(80);
AsyncWebSocket ws("/data/ws");

// char* hostName = "CleargrassTermostat";
char hostName[] = "CleargrassTermostat";

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

int SLEEP_TIME = 10; // seconds

#define BLUETOOTH_SCAN_TIME 10 // seconds

Thermostat termostat;
Relay relay;
// define files

File devicesF;
File roomsF;

String floatToString(float f)
{
  String s = String(f, 1);
  return s;
}



void loadDevicesToMemory() {
devicesF = LittleFS.open("/devices.json", "r");
if (!devicesF) {
  Serial.println(F("Failed to open devices.json for reading"));
} else {
  DeserializationError error = deserializeJson(devices, devicesF);
  Serial.println(F("Devices loaded"));
  serializeJson(devices, Serial);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
  }
  devicesF.close();
}
}

void loadRoomsToMemory()
{
  roomsF = LittleFS.open("/rooms.json", "r");
  if (!roomsF)
  {
    Serial.println(F("Failed to open rooms.json for reading"));
  }
  else
  {
    DeserializationError error = deserializeJson(rooms, roomsF);
    Serial.println(F("Rooms loaded"));
    serializeJson(rooms, Serial);
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    devicesF.close();
  }
}



// function to save data to file
bool saveFile(String path, String data)
{
  File file = LittleFS.open(path, "w");
  if (!file)
  {
    Serial.println(F("Failed to open file for writing"));
    return false;
  }
  else
  {
    file.print(data);
    Serial.print(F("File saved: "));
    file.close();
    return true;
  }
};
void saveDevicesToFile()
{
  Serial.println(F(":: Saving Devices to file ::"));

  String jsonStr;
  serializeJson(devices, jsonStr);
  saveFile("/devices.json", jsonStr);
}

void listAllFilesInDir(const char *dirname, Stream &out)
{
  out.printf("Listing directory: %s\r\n", dirname);
  File root = LittleFS.open(dirname);
  if (!root)
  {
    out.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    out.println(" - not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      out.print("  DIR : ");
      out.println(file.name());
      listAllFilesInDir(file.path(), Serial);
    }
    else
    {
      out.print("  FILE: ");
      out.print(file.name());
      out.print("\tSIZE: ");
      out.println(file.size());
    }
    file = root.openNextFile();
  }
}

String getTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println(F("Failed to obtain time"));
    return "";
  }
  char timeStringBuff[50]; // 50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%d.%m.%Y %H:%M:%S", &timeinfo);
  String time(timeStringBuff);
  return time;
}

bool initTime()
{
  // init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Current device time: " + getTime());
  return true;
}

void sleepTrigger()
{
#if SLEEP_TIME > 0
  // esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000); // translate second to
  // micro second
  Serial.printf("Enter deep sleep for %d seconds...\n\r", (SLEEP_TIME));
  delay(10);
  // esp_deep_sleep_start();
  esp_deep_sleep(1000000LL * SLEEP_TIME);
  Serial.println(F("After deep sleep"));
#endif
}



