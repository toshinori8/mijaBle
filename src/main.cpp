#include <Arduino.h>

// /*
//   Code based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleScan.cpp
//   Ported to Arduino ESP32 by Evandro Copercini and based on https://github.com/turlvo/KuKuMi/
//   Additional improvements by Alexander Savychev https://github.com/save2love/mi-ble-mqtt
// */
// #include <BLEDevice.h>
// #include <BLEUtils.h>
// #include <BLEScan.h>
// #include <BLEAdvertisedDevice.h>

// #include <WiFi.h>
// #include <WiFiMulti.h>


// #include "soc/rtc_cntl_reg.h"
// #include "esp_system.h"
// #include "const.h"

// // WiFi & BT Instance
// WiFiMulti wifiMulti;
// WiFiClient espClient;
// // PubSubClient client(espClient);
// BLEScan *pBLEScan;

// void IRAM_ATTR resetModule() {
//   ets_printf("reboot\n");
//   //esp_restart_noos();
//   //ESP.restart();
//   esp_restart();
// }

// void blinkLed(uint8_t repeats, uint16_t delayTime) {
//   for (uint i = 0; i < repeats; i++) {
//     digitalWrite(PIN_LED, HIGH);
//     delay(delayTime);
//     digitalWrite(PIN_LED, LOW);
//     delay(delayTime);
//   }
// }

// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
//     uint8_t getServiceData(uint8_t uuid_len, uint8_t* payload, uint8_t length, uint8_t* data) {
//       if (length < uuid_len)
//         return 0;
//       if (length > uuid_len) {
//         memcpy(data, payload + uuid_len, length - uuid_len);
//         return length - uuid_len;
//       }
//       return 0;
//     }

//     uint8_t tryParsePayload(uint8_t* payload, size_t payload_len, uint8_t* service_data) {
//       int8_t length;
//       uint8_t ad_type;
//       uint8_t sizeConsumed = 0;
//       bool finished = false;

//       while (!finished) {
//         length = *payload;          // Retrieve the length of the record.
//         payload++;                  // Skip to type
//         sizeConsumed += 1 + length; // increase the size consumed.

//         if (length != 0) { // A length of 0 indicates that we have reached the end.
//           ad_type = *payload;
//           payload++;
//           length--;

//           switch (ad_type) {
//             case ESP_BLE_AD_TYPE_SERVICE_DATA: {  // 2 byte UUID
//                 return getServiceData(2, payload, length, service_data);
//               }

//             case ESP_BLE_AD_TYPE_32SERVICE_DATA: { // 4 byte UUID
//                 return getServiceData(4, payload, length, service_data);
//               }

//             case ESP_BLE_AD_TYPE_128SERVICE_DATA: { // 16 byte UUID
//                 return getServiceData(16, payload, length, service_data);
//               }
//           }
//           payload += length;
//         }

//         if (sizeConsumed >= payload_len)
//           finished = true;
//       }
//     }

//     uint16_t readValue(uint8_t* data, uint8_t i1, uint8_t i2) {
//       uint16_t value = 0;
//       value |= data[i1] << 8;
//       value |= data[i2];
//       return value;
//     }

//     void publish(const char* addr, char* valueName, char* value) {
//       char topic[50];
//       sprintf(topic, "%s/%s/%s", MQTT_PREFIX, addr, valueName);
//       // if (client.connected()) {
//       //   client.publish(topic, value);
//       //   client.loop();
//       // }
//       Serial.print(topic);
//       Serial.print(F(" = "));
//       Serial.print(value);
//       Serial.println();
//     }

//     void processData(XiaomiDevice device, uint8_t* payload, size_t len, char* addr) {
//       uint8_t data[30];
//       uint8_t raw_data_length = tryParsePayload(payload, len, data);
//       if (raw_data_length == 0) {
//         Serial.println(F("Skip! No service date"));
//         return;
//       }

//       bool has_data = data[0] & 0x40;
//       bool has_capability = data[0] & 0x20;
//       bool has_encryption = data[0] & 0x08;

//       if (!has_data) {
//         Serial.println(F("Skip! Service data has no DATA flag"));
//         return;
//       }

//       if (has_encryption) {
//         Serial.println(F("Skip! Service data is encrypted"));
//         return;
//       }

//       /*static uint8_t last_frame_count = 0;
//         if (last_frame_count == data[4]) {
//         Serial.println(F("Skip! Duplicate data packet received"));
//         return;
//         }
//         last_frame_count = data[4];*/

//       // Check device signature
//       if (data[2] != device.sign1 || data[3] != device.sign2) {
//         Serial.println(F("Skip! Wrong device signature"));
//         return;
//       }

//       uint16_t temp, humd;
//       uint8_t bat;
//       char value[10];

//       uint8_t data_offset = has_capability ? 12 : 11;
//       raw_data_length = raw_data_length - data_offset;

//       while (raw_data_length > 0) {
//         if (data[data_offset + 1] != 0x10) {
//           Serial.println(F("Skip! Fixed byte (0x10) not found"));
//           break;
//         }

//         const uint8_t value_length = data[data_offset + 2];
//         if ((value_length < 1) || (value_length > 4) || (raw_data_length < (3 + value_length))) {
//           Serial.printf("Skip! Value has wrong size (%d)!", value_length);
//           break;
//         }

//         const uint8_t value_type = data[data_offset + 0];
//         //const uint8_t *data = &data[data_offset + 3];

//         switch (data[data_offset]) {
//           case 0x04:
//             temp = readValue(data, data_offset + 4, data_offset + 3);
//             sprintf(value, "%.1f", temp / 10.0);
//             publish(addr, "temp", value);
//             break;

//           case 0x06:
//             humd = readValue(data, data_offset + 4, data_offset + 3);
//             sprintf(value, "%.1f", humd / 10.0);
//             publish(addr, "humd", value);
//             break;

//           case 0x0A:
//             bat = data[data_offset + 3];
//             sprintf(value, "%d", bat);
//             publish(addr, "battery", value);
//             break;

//           case 0x0D:
//             temp = readValue(data, data_offset + 4, data_offset + 3);
//             sprintf(value, "%.1f", temp / 10.0);
//             publish(addr, "temp", value);
//             humd = readValue(data, data_offset + 6, data_offset + 5);
//             sprintf(value, "%.1f", humd / 10.0);
//             publish(addr, "humd", value);
//             break;

//           default:
//             Serial.print(F("Unknown value type: "));
//             Serial.print(data[data_offset]);
//             break;
//         }

//         raw_data_length -= 3 + value_length;
//         data_offset += 3 + value_length;
//       }
//     }

//     uint8_t getDeviceIndexByName(std::string name) {
//       for (int i = 0; i < xiaomiDevicesSize; i++) {
//         if (!name.compare(xiaomiDevices[i].name)) {
//           return i;
//         }
//       }
//       return 0xFF;
//     }

//     void onResult(BLEAdvertisedDevice advertisedDevice)
//     {
//       if (!advertisedDevice.haveName() || !advertisedDevice.haveServiceData())
//         return;

//       std::string dname = advertisedDevice.getName();
//       Serial.print(F("Device: "));
//       Serial.print(dname.c_str());

//       uint8_t dIndex = getDeviceIndexByName(dname);
//       if (dIndex >= xiaomiDevicesSize) {
//         Serial.println(F(" Not supported!"));
//         return;
//       }
//       Serial.println(F(" OK!"));

//       XiaomiDevice device = xiaomiDevices[dIndex];

//       size_t len = advertisedDevice.getPayloadLength();
//       uint8_t* payload = advertisedDevice.getPayload();
//       char addr[20];
//       strcpy(addr, advertisedDevice.getAddress().toString().c_str());

//       digitalWrite(PIN_LED, HIGH);
//       processData(device, payload, len, addr);
//       digitalWrite(PIN_LED, LOW);
//     }
// };

// void WiFiEvent(WiFiEvent_t event)
// {
//   digitalWrite(PIN_LED, HIGH);
//   switch (event) {
//     case SYSTEM_EVENT_WIFI_READY: Serial.println(F("WiFi ready")); break;
//     case SYSTEM_EVENT_SCAN_DONE: Serial.println(F("WiFi finish scanning")); break;
//     case SYSTEM_EVENT_STA_CONNECTED: Serial.println(F("WiFi connected")); break;
//     case SYSTEM_EVENT_STA_GOT_IP:
//       Serial.print(F("WiFi got IP: "));
//       Serial.println(WiFi.localIP());
//       break;
//     case SYSTEM_EVENT_STA_DISCONNECTED:
//       Serial.println(F("WiFi lost connection"));
//       blinkLed(5, 250);
//       resetModule();
//       break;
//     default:
//       Serial.print(F("WiFi "));
//       Serial.println(event);
//       break;
//   }
//   delay(10);
//   digitalWrite(PIN_LED, LOW);
// }

// void reconnect() {
//   // Loop until we're reconnected
//   // while (!client.connected()) {
//   //   Serial.print(F("Attempting MQTT connection..."));

//   //   if (client.connect(MQTT_GW_NAME)) {
//   //     Serial.println(F("connected"));
//   //   } else {
//   //     Serial.print(F("failed, rc="));
//   //     Serial.print(client.state());
//   //     Serial.println(F(" try again in 5 seconds"));
//   //     blinkLed(10, 500);
//   //   }
//   // }
// }



// void setup()
// {
//   WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector

//   Serial.begin(115200);

//   pinMode(PIN_LED, OUTPUT);

//   // BLE init and setting
//   BLEDevice::init("");
//   pBLEScan = BLEDevice::getScan(); //create new scan
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
//   pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster
//   pBLEScan->setInterval(0x50);
//   pBLEScan->setWindow(0x30);

//   // WiFi Setting and Connecting
//   WiFi.onEvent(WiFiEvent);
//   wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

//   Serial.println();

//   int wifi_retry = 10;
//   while (--wifi_retry >= 0) {
//     if (wifiMulti.run() == WL_CONNECTED)
//       break;

//     if (wifi_retry == 0)
//       resetModule();

//     delay(3000);
//   }

//   //client.setServer(MQTT_SERVER, MQTT_PORT);
// }

// void loop() {
//   // if (!client.connected())
//   //   reconnect();
//   // client.loop();

//   Serial.printf("Start BLE scan for %d seconds...\n", SCAN_TIME);

//   BLEScanResults foundDevices = pBLEScan->start(SCAN_TIME);
//   int count = foundDevices.getCount();
//   printf("Found device count : %d\n", count);

// #if SLEEP_TIME > 0
//   //esp_sleep_enable_timer_wakeup(SLEEP_TIME * 1000000); // translate second to micro second
//   Serial.printf("Enter deep sleep for %d seconds...\n\r", (SLEEP_TIME));
//   delay(10);
//   //esp_deep_sleep_start();
//   esp_deep_sleep(1000000LL * SLEEP_TIME);
//   Serial.println(F("After deep sleep"));
// #endif
// }


// Load Wi-Fi library
#include <WiFi.h>
#include <WebServer.h>

// bluetooth stuff
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

//sstream
#include <sstream>

//time
#include "time.h"

// Replace with your network credentials

const char* ssid     = "|oooooi|";
const char* password = "pmgana921";

// Set web server port number to 80
WebServer server(80);

//#define LED 1            //builtin led
#define BLUETOOTH_SCAN_TIME  10 // seconds

long previousMillis = 0;        // last time
long interval = 1000 * 20;           // interval at which to scan for sensor data
                                      //each 20 seconds scan for 10 seconds
                                      //so 10 seconds stay free for wifi connection then

BLEScan *pBLEScan;

const int maxDataCount = 15;
String myData[ maxDataCount ][ 7 ]; //15 sensors at max at the moment
  //0 = mac
  //1 = temp
  //2 = hum
  //3 = bat
  //4 = tempTimestamp
  //5 = humTimestamp
  //6 = batTimestamp

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

String getTime()
{
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return "";
  }
  
  char timeStringBuff[50]; //50 chars should be enough
  strftime(timeStringBuff, sizeof(timeStringBuff), "%d.%m.%Y %H:%M:%S", &timeinfo);
  String time(timeStringBuff);
  
  return time;
}

String floatToString(float f)
{
  String s = String(f,1);
  return s;
}




String printData(int i)
{
  if (myData[i][0] != NULL)
    return myData[i][0] + ", Temp: " + myData[i][1] + ", Hum: " + myData[i][2] + ", Bat: " + myData[i][3]  + ", tempDate: " + myData[i][4]  + ", humDate: " + myData[i][5]  + ", batDate: " + myData[i][6];
  else
    return "";
}
String printAllData()
{
  String result;
  
  // loop through array's rows
  for ( int i = 0; i < maxDataCount; ++i )
  {
    result += printData(i) + "<br>";
  }

  return result;
}
String printAllDataJson()
{
  String result = "{\"data\": [";
  
  // loop through array's rows
  for ( int i = 0; i < maxDataCount; ++i )
  {
    if (myData[i][0] != NULL)
    {    
      result += "{";
      result += "\"mac\": \"" + myData[i][0] + "\",";
      result += "\"temp\": \"" + myData[i][1] + "\",";
      result += "\"hum\": \"" + myData[i][2] + "\",";
      result += "\"bat\": \"" + myData[i][3] + "\",";
      result += "\"tempDate\": \"" + myData[i][4] + "\",";
      result += "\"humDate\": \"" + myData[i][5] + "\",";
      result += "\"batDate\": \"" + myData[i][6] + "\"";
      result += "}";

      //add delimiter
      result += ",";
    }
  }

  //delete last char if it's a ","
  if (result.endsWith(","))
    result = result.substring(0, result.length() - 1);

  result += "]}";

  return result;
}


void setData(String mac, float temp, float hum, float bat)
{
  int index;
  
  //lets search if we already have data for this mac address somewhere
  for ( int i = 0; i < maxDataCount; ++i )
  {
    //if we either found the mac address in our already existing data
    //or we found a first slot that is currently empty
    if (myData[i][0] == mac || myData[i][0] == NULL)
    {
      //we found the correct index for storing our data
      index = i;
      
      //leave the loop
      break;
    }   
  }

  //finally set the data
  String currentDate = getTime();
  myData[index][0] = mac;
  if (temp != NULL)
  {
    myData[index][1] = floatToString(temp);
    myData[index][4] = currentDate;
  };
  
  if (hum != NULL)
  {
    myData[index][2] = floatToString(hum);
    myData[index][5] = currentDate;
  };
  
  if (bat != NULL)
  {
    myData[index][3] = floatToString(bat);
    myData[index][6] = currentDate;
  };

  //print the data to the serial monitor
  Serial.println(printData(index));
}
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        String mac = advertisedDevice.getAddress().toString().c_str();

        //we're only interested in bluetooth devices sending service data 
        //and if the service uuid always starts with "0000fe95-0000-1000-8000-00805f9b34fb"
        if (advertisedDevice.haveServiceData() && advertisedDevice.getServiceDataUUID().toString() == "0000fe95-0000-1000-8000-00805f9b34fb")
        {
            /*
            Serial.println("");
            Serial.print("Device ");
            Serial.println(advertisedDevice.toString().c_str());
    
            Serial.print("Device Name: ");
            Serial.println(advertisedDevice.getName().c_str());
            Serial.print("Device Mac: ");
            Serial.println(advertisedDevice.getAddress().toString().c_str());
                
            if (advertisedDevice.haveServiceUUID())
            {
              Serial.print("Service UUID:");
              Serial.println(advertisedDevice.getServiceUUID().toString().c_str());
            }
    
            if (advertisedDevice.haveServiceData())
            {
              Serial.print("Service Data:");
              Serial.println(advertisedDevice.getServiceData().c_str());
    
              Serial.print("Service Data UUID:");
              Serial.println(advertisedDevice.getServiceDataUUID().toString().c_str());
            }
    
            if (advertisedDevice.haveManufacturerData())
            {
              Serial.print("Device Manufacturer Data:");
              Serial.println(advertisedDevice.getManufacturerData().c_str());
            }
    
            if (advertisedDevice.haveRSSI())
            {
              Serial.print("Device RSSI:");
              Serial.println(advertisedDevice.getRSSI());
            }
            */
            
            std::string strServiceData = advertisedDevice.getServiceData();
            uint8_t cServiceData[100];
            char charServiceData[100];

            strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);
          
            for (int i=0;i<strServiceData.length();i++)
            {
                sprintf(&charServiceData[i*2], "%02x", cServiceData[i]);
            }

            std::stringstream ss;
            ss << "fe95" << charServiceData;
            
            Serial.print("Payload:");
            Serial.println(ss.str().c_str());

            unsigned long value, value2;
            char charValue[5] = {0,};

            Serial.print("Message: ");
            Serial.println(cServiceData[11]);
          
            switch (cServiceData[11])
            {
                case 0x04: //4  //only temp
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);  
                    setData(mac, (float)value/10, NULL, NULL); 
                    break;
                case 0x06: //6  //only hum
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);               
                    setData(mac, NULL, (float)value/10, NULL);
                    break;
                case 0x0A: //10  //only battery
                    sprintf(charValue, "%02X", cServiceData[14]);
                    value = strtol(charValue, 0, 16);                
                    setData(mac, NULL, NULL, (float)value);
                    break;
                case 0x0D: //13  //battery and hum
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);                                    
                    sprintf(charValue, "%02X%02X", cServiceData[17], cServiceData[16]);
                    value2 = strtol(charValue, 0, 16);                               
                    setData(mac, (float)value/10, (float)value2/10, NULL);
                    break;
            }
        }
    }
};

void initBluetooth()
{
    BLEDevice::init("");
    pBLEScan = BLEDevice::getScan(); //create new scan
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), true);  //true needed to get multiple messages per mac address
    pBLEScan->setActiveScan(false); //active scan uses more power, but get results faster
}


static void scanCompleteCB(BLEScanResults scanResults)
{
  Serial.println("Scan complete!");
}



void setup() 
{
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Connected to WiFi..");
  }

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Current device time: " + getTime());

  //refreshData
  server.on("/json", []()
  {
    String html = printAllDataJson();
    
    //print data as json
    server.send(200, "text / plain", html);
  });

  //root
  server.on("/", []()
  {
    String html = "<html>";
      html += "<body><h1>esp32_cleargrass_wifi Webinterface</h1><br><br>";
      html += "Current device time: " + getTime();
      html += "<br><br>";    
      html += printAllData();
           
      html +="</body></html>";
    
    server.send(200, "text / plain", html);
  });

  //start the webserver
  server.begin();

  //bluetooth
  initBluetooth();
}

void loop()
{
  //handle webserver requests
  server.handleClient();

  //timer
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    //scan for the sensors asynchronously
    Serial.printf("Start BLE scan for %d seconds...\n", BLUETOOTH_SCAN_TIME);
    pBLEScan->start(BLUETOOTH_SCAN_TIME, scanCompleteCB);
  }
}


