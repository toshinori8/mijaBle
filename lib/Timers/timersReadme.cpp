
// This is a sample sketch for the Timers library
#include "timers.h"

// Define the number of timers
Timers<4> timers;

void setup()
{

    Serial.begin(115200);

    // Attach function's to a timer
    timers.attach(0, 1000, function01);
    // Define [onTime] called when a timer is triggered
    timers.onTime = onTimer;
}

void loop()
{
    // Process timers
    timers.process();
    delay(10);
}

void function01()
{
    Serial.println("function01");
}
