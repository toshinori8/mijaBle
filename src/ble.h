#include <Arduino.h>

// bluetooth stuff
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ArduinoJson.h> 

#define BLUETOOTH_SCAN_TIME  10 // seconds
DynamicJsonDocument devices(2048); // Json document for storing devices data 


long previousMillis = 0;              // last time
long interval = 1000 * 20;            // interval at which to scan for sensor data                                      //each 20 seconds scan for 10 seconds
                                      // so 10 seconds stay free for wifi connection then

BLEScan *pBLEScan;

  
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;



String getTime(){
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

String floatToString(float f){
  String s = String(f,1);
  return s;
}



void updateDevice(String mac, String type, float data){

  bool deviceFound = false;

  for (int i = 0; i < devices.size(); i++) {
    if (devices[i]["mac"] == mac) {
      deviceFound = true;
      if(data){devices[i][type]  = data;}
      break;
    }
  }

  if (!deviceFound) {
    JsonObject device = devices.createNestedObject();
    device["mac"] = mac;
    if(data){device[type]  = data;}
  }


};



class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks
{
    void onResult(BLEAdvertisedDevice advertisedDevice)
    {
        
        String mac = advertisedDevice.getAddress().toString().c_str();
      
        
        //we're only interested in bluetooth devices sending service data 
        //and if the service uuid always starts with "0000fe95-0000-1000-8000-00805f9b34fb"
        if (advertisedDevice.haveServiceData() && advertisedDevice.getServiceDataUUID().toString() == "0000fe95-0000-1000-8000-00805f9b34fb")
        {
            //get the service data
            std::string strServiceData = advertisedDevice.getServiceData();
            uint8_t cServiceData[100];
            char charServiceData[100];
            //copy the service data to a char array
            strServiceData.copy((char *)cServiceData, strServiceData.length(), 0);
            //convert the service data to a hex string
            for (int i=0;i<strServiceData.length();i++)
            {
                sprintf(&charServiceData[i*2], "%02x", cServiceData[i]);
            }
            
            std::stringstream ss;
            ss << "fe95" << charServiceData;
            
            // Serial.print("Payload:");
            // Serial.println(ss.str().c_str());

            //
            unsigned long value, value2;
            char charValue[5] = {0,};

            //Serial.print("Message: ");
            //Serial.println(cServiceData[11]);

            switch (cServiceData[11])
            {   
                case 0x04: //4  //only temp
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);  

                    updateDevice(mac, "temp", (float)value/10);
                    
                    break;
                case 0x06: //6  //only hum
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);               

                    updateDevice(mac, "hum", (float)value/10);

                    break;
                case 0x0A: //10  //only battery
                    sprintf(charValue, "%02X", cServiceData[14]);
                    value = strtol(charValue, 0, 16);                

                    updateDevice(mac, "bat", (float)value/10);

                    break;
                case 0x0D: //13  //battery and hum
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);                                    
                    sprintf(charValue, "%02X%02X", cServiceData[17], cServiceData[16]);
                    value2 = strtol(charValue, 0, 16);                               

                    updateDevice(mac, "hum", (float)value/10);
                    updateDevice(mac, "bat", (float)value2/10);

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

// Scan

static void scanCompleteCB(BLEScanResults scanResults)
{
  Serial.println("Scan complete!");
}




// Loop 
void ble_scan_loop(){

  // timer 
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval)
  {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    //scan for the sensors asynchronously
    Serial.printf("Start BLE scan for %d seconds...\n", BLUETOOTH_SCAN_TIME);
    pBLEScan->start(BLUETOOTH_SCAN_TIME, scanCompleteCB);
  }


};


