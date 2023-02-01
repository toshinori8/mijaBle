
class Thermostat
{
private:
    int minTemperature;
    int maxTemperature;

    JsonObject rooms;
    JsonObject devices;

public:
    // konstruktor
    Thermostat()
    {
        this->rooms = rooms; 
        this->devices = devices;
   }
 

    // funkcja do przetwarzania danych
    void processData()
    {
        Serial.println("Processing data...");
        
        // display  devices JSON object in serial 
        serializeJson(devices, Serial);
        Serial.println();
        


        // przetwarzanie danych
    }

    // funkcja do wysyłania danych
    void sendData()
    {
        // wyślij dane
    }
};