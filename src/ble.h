#include <Arduino.h>

// bluetooth stuff
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <ArduinoJson.h> 

#define BLUETOOTH_SCAN_TIME  10 // seconds

DynamicJsonDocument doc(2048); // Json document for storing devices data 

long previousMillis = 0;              // last time
long interval = 1000 * 20;            // interval at which to scan for sensor data                                      //each 20 seconds scan for 10 seconds
                                      // so 10 seconds stay free for wifi connection then

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
    
    return myData[i][0] + ", | Temp: " + myData[i][1] + ", Hum: " + myData[i][2] + ", Bat: " + myData[i][3];
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

void addDataToJson(String mac, float temp, float hum, float bat, String name) {
  bool deviceFound = false;
  
  for (int i = 0; i < doc.size(); i++) {
    if (doc[i]["mac"] == mac) {
      deviceFound = true;
      if(temp != 0){doc[i]["temp"]  = temp;}
      if(hum  != 0){doc[i]["hum"]   = hum;}
      if(bat  != 0){doc[i]["bat"]   = bat;}
      if(name != "")  {doc[i]["name"]  = name;}  
      break;
    }
  }
  if (!deviceFound) {

    JsonObject device = doc.createNestedObject();
    
    
      device["mac"] = mac;
      if(temp != 0){device["temp"]  = temp;}
      if(hum  != 0){device["hum"]   = hum;}
      if(bat  != 0){device["bat"]   = bat;}
      if(name != "")  {device["name"]  = name;}  
  }
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
  if (temp != 0)
  {
    myData[index][1] = floatToString(temp);
    myData[index][4] = currentDate;
  };
  
  if (hum != 0)
  {
    myData[index][2] = floatToString(hum);
    myData[index][5] = currentDate;
  };
  
  if (bat != 0)
  {
    myData[index][3] = floatToString(bat);
    myData[index][6] = currentDate;
  };


  //print the data to the serial monitor
  //Serial.println(printData(index));
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
                    setData(mac, (float)value/10, NULL, NULL); 

                    addDataToJson(mac, (float)value/10, NULL, NULL, "");
                    break;
                case 0x06: //6  //only hum
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);               
                    setData(mac, NULL, (float)value/10, NULL);

                    addDataToJson(mac, NULL, (float)value/10, NULL, "");
                    break;
                case 0x0A: //10  //only battery
                    sprintf(charValue, "%02X", cServiceData[14]);
                    value = strtol(charValue, 0, 16);                
                    setData(mac, NULL, NULL, (float)value);

                    addDataToJson(mac, NULL, NULL, (float)value, "");
                    break;
                case 0x0D: //13  //battery and hum
                    sprintf(charValue, "%02X%02X", cServiceData[15], cServiceData[14]);
                    value = strtol(charValue, 0, 16);                                    
                    sprintf(charValue, "%02X%02X", cServiceData[17], cServiceData[16]);
                    value2 = strtol(charValue, 0, 16);                               
                    setData(mac, (float)value/10, (float)value2/10, NULL);
                    
                    addDataToJson(mac, (float)value/10, (float)value2/10, NULL, "");
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


