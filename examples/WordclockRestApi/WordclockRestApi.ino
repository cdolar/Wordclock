#include "Configuration.h"
#include "RealTimeClock.h"
#include "Webserver.h"

using namespace wordclock;

void setup()
{
    Serial.begin(115200);
    
    Serial.println("Setting up RealTimeClock");
    if (rtc.init())
    {
        Serial.println("...successful");
    }
    else
    {
        Serial.println("...not successful");
    }

    Serial.println("Setting up Webserver");
    webserver.init();

}

void loop()
{
    bool hasNewTime = rtc.hasNewTime();
    webserver.loop();

}