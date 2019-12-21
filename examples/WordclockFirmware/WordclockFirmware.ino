#include "Wordclock.h"
#include "RealTimeClock.h"
#include "Configuration.h"
#include "Webserver.h"

#include <NeoPixelBus.h>

typedef LedMatrixConfig_DE_11x10_reversed       led_matrix_config_t;
typedef MatrixEmulator<led_matrix_config_t>     matrixEmu_t;
typedef Wordclock<led_matrix_config_t>          wordclock_t;

wordclock_t mywordclock;
matrixEmu_t matrixEmu;

// three element pixels, in different order and speeds
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> strip(wordclock_t::SIZE);

using namespace wordclock;

void setup()
{
    Serial.begin(115200);
    delay(3000); // Wait on terminal

    Serial.println("Restoring configuration from flash");
    configurationSetup();
    Serial.println("The Configuration looks like this:");
    config.toSerial();

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
    for (uint8_t h=0; h<23; ++h)
    {
        for (uint8_t m=0; m<60; m+=5)
        {
            Serial.print("Current Hours ");
            Serial.print(h);
            Serial.print(":");
            Serial.print(m);
            Serial.println("");
            Serial.flush();
            matrixEmu.clear();
            mywordclock.setTime(h, m, matrixEmu, matrixEmu_t::StdColor);
            matrixEmu.print();
            delay(100);
        }
    }
}