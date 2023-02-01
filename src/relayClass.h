class Relay
{
private:
public:
    static const int zonesCount = 8;        // number of zones
    bool invert = false;                    // valves are NO or NC

    struct zones                            
    {
        int roomID;
        bool state;
    };
    zones zone[zonesCount];                 
    
    
    void printZones();                      // print all zones
    void switchZone(int roomID, bool state);


};





void Relay::printZones()
{
    for (int i = 0; i < zonesCount; i++)
    {
        Serial.println("----------------------");
        Serial.println("Relay roomID: " + String(zone[i].roomID) + " on: " + String(zone[i].state ? "true" : "false"));
    }
}

void Relay::switchZone(int targetRoomID, bool state)
{

    zone[targetRoomID].state = state;
    //printZones();
}
