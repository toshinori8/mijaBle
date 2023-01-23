#include <Arduino.h>
#include <ArduinoJson.h>
#include <BLEAdvertisedDevice.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEUtils.h>
#include <sstream>
#include "main.h"




long previousMillis = 0; // last time
long interval =
    1000 *
    20; // interval at which to scan for sensor data //each 20 seconds scan for
        // 10 seconds so 10 seconds stay free for wifi connection then
// Load from littlefs devices.json

void loadDevices() {
  devicesF = LittleFS.open("/devices.json", "r");
  if (!devicesF) {
    Serial.println("Failed to open devices.json for reading");
  } else {
    DeserializationError error = deserializeJson(devices, devicesF);

    Serial.println("Devices loaded");
    // print devices content
    serializeJson(devices, Serial);
    Serial.println();

    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
    }
    devicesF.close();
  }
}

BLEScan *pBLEScan;

void updateDevice(String mac, String type, float data) {

  
  String dataString = floatToString(data);

  bool deviceFound = false;

  for (int i = 0; i < devices.size(); i++) {
    if (devices[i]["mac"] == mac) {
      deviceFound = true;
      if (data) {
        devices[i][type] = dataString;
        devices[i]["lastSeen"] = getTime();
      }
      break;
    }
  }

  if (!deviceFound) {
    JsonObject device = devices.createNestedObject();
    device["mac"] = mac;
    if (data) {
      device["lastSeen"] = getTime();
      device[type] = dataString;
    }
  }
};

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {

    String mac = advertisedDevice.getAddress().toString().c_str();
    // we're only interested in bluetooth devices sending service data
    // and if the service uuid always starts with
    // "0000fe95-0000-1000-8000-00805f9b34fb"
    if (advertisedDevice.haveServiceData() &&
        advertisedDevice.getServiceDataUUID().toString() ==
            "0000fe95-0000-1000-8000-00805f9b34fb") {
      // get the service data
      std::string strServiceData = advertisedDevice.getServiceData();
      uint8_t cServiceData[100];
      char charServiceData[100];
      // copy the service data to a char array
      strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);
      // convert the service data to a hex string
      for (int i = 0; i < strServiceData.length(); i++) {
        sprintf(&charServiceData[i * 2], "%02x", cServiceData[i]);
      }

      std::stringstream ss;
      ss << "fe95" << charServiceData;

      // Serial.print("Payload:");
      // Serial.println(ss.str().c_str());

      //
      unsigned long value, value2;
      char charValue[5] = {
          0,
      };
      float roundedValue = 0.0;
      // Serial.print("Message: ");
      // Serial.println(cServiceData[11]);

      switch (cServiceData[11]) {
      case 0x04: // 4  //only temp
        sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
        value = strtol(charValue, 0, 16);
        Serial.println("charValue");
        Serial.println(value / 10);
        roundedValue = round((float)value / 10 * 100.0) / 100.0;
        updateDevice(mac, "temp", roundedValue);

        break;
      case 0x06: // 6  //only hum
        sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
        value = strtol(charValue, 0, 16);
        roundedValue = round((float)value / 10 * 10.0) / 10.0;
        updateDevice(mac, "hum", roundedValue);

        break;
      case 0x0A: // 10  //only battery
        sprintf(charValue, "%02X", cServiceData[14]);
        value = strtol(charValue, 0, 16);
        Serial.println("charValue");
        Serial.println(value / 10);
        roundedValue = round((float)value / 10 * 10.0) / 10.0;
        updateDevice(mac, "bat", roundedValue);

        break;
      case 0x0D: // 13  //battery and hum
        sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
        value = strtol(charValue, 0, 16);
        sprintf(charValue, "%02X%02X", cServiceData[17], cServiceData[16]);
        value2 = strtol(charValue, 0, 16);

        // updateDevice(mac, "hum", (float)value/10);
        // updateDevice(mac, "bat", (float)value2/10);

        break;
      }
    }
  }
};

void initBluetooth() {
  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); // create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(
      new MyAdvertisedDeviceCallbacks(),
      true); // true needed to get multiple messages per mac address
  pBLEScan->setActiveScan(
      false); // active scan uses more power, but get results faster
}

// Scan

static void scanCompleteCB(BLEScanResults scanResults) {
  Serial.println("Scan complete!");
}

// Loop
void ble_scan_loop() {

  // timer
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // scan for the sensors asynchronously
    Serial.printf("Start BLE scan for %d seconds...\n", BLUETOOTH_SCAN_TIME);
    pBLEScan->start(BLUETOOTH_SCAN_TIME, scanCompleteCB);
  }
};

// class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
//     uint8_t getServiceData(uint8_t uuid_len, uint8_t* payload, uint8_t
//     length, uint8_t* data) {
//       if (length < uuid_len)
//         return 0;
//       if (length > uuid_len) {
//         memcpy(data, payload + uuid_len, length - uuid_len);
//         return length - uuid_len;
//       }
//       return 0;
//     }

//     uint8_t tryParsePayload(uint8_t* payload, size_t payload_len, uint8_t*
//     service_data) {
//       int8_t length;
//       uint8_t ad_type;
//       uint8_t sizeConsumed = 0;
//       bool finished = false;

//       while (!finished) {
//         length = *payload;          // Retrieve the length of the record.
//         payload++;                  // Skip to type
//         sizeConsumed += 1 + length; // increase the size consumed.

//         if (length != 0) { // A length of 0 indicates that we have reached
//         the end.
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

//

//     void processData(XiaomiDevice device, uint8_t* payload, size_t len, char*
//     addr) {
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
//         if ((value_length < 1) || (value_length > 4) || (raw_data_length < (3
//         + value_length))) {
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
//       if (!advertisedDevice.haveName() ||
//       !advertisedDevice.haveServiceData())
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
