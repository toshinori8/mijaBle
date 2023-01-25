
// This is a sample sketch for the Timers library
#include "timers.h"

// Define the number of timers
Timers<4> timers;

void function01()
{
    Serial.println("function01");
}


void setup()
{

    Serial.begin(115200);
    // Attach function's to a timer

    timers.attach(1,1000,function01);
    // Define [onTime] called when a timer is triggered

}

void loop()
{
    // Process timers
    timers.process();
    delay(10);
}


