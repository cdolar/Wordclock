#include <Configuration.h>

void setup()
{
    Serial.begin(115200);

    wordclock::configurationSetup();

    Serial.println("The Configuration looks like this:");
    wordclock::config.toSerial();

    Serial.println("Now lets change something... hit any key: ");
    Serial.flush();
    while (Serial.available() <= 0) {
        ;
    }
    char inByte = Serial.read();
    wordclock::config.hostname[0]=inByte;
    wordclock::saveConfiguration();

    Serial.println("Now the Configuration looks like this:");
    wordclock::config.toSerial();

}

void loop()
{
    // do nothing
}