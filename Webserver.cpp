#include "Webserver.h"
#include "ESP8266WebServer.h"
#include "Configuration.h"
#include "RealTimeClock.h"


namespace wordclock
{
    namespace api
    {
        void getStatus()
        {
            webserver.apiGetStatus();
        }

        void setDateTime()
        {
            webserver.apiSetDateTime();
        }

        void setBrightness()
        {
            webserver.apiSetBrightness();
        }
    }


    Webserver::Webserver(int listen_port)
    : m_server(listen_port)
    {}

    bool Webserver::init()
    {
        bool retVal = false;
        WiFi.begin();
        Serial.println("Activating access point");
        String ssid = "WordClock-" + String(ESP.getChipId());

        Serial.print("SSID: ");
        Serial.print(ssid.c_str());
        Serial.println();

        retVal = WiFi.softAP(ssid.c_str());
        if (retVal)
        {
            Serial.println("Setting up Access Point successful");
            Serial.println(WiFi.softAPIP());
            
            Serial.println("Setting up DNS server");


            Serial.println("Setting up web api");
            m_server.on("/api/status", api::getStatus);
            m_server.on("/api/setDateTime", api::setDateTime);
            m_server.on("/api/setBrightness", api::setBrightness);
            m_server.begin();

            Serial.print("Setting up MDNS with hostname ");
            Serial.print(config.hostname);
            Serial.println();
            retVal = MDNS.begin(config.hostname);
            if (!retVal)
            {
                Serial.println("... not successful");
            }
        }
        else
        {
            Serial.println("Wifi access Point setup unsuccessful");
        }
        return retVal;
    }

    void Webserver::loop()
    {
        m_server.handleClient();
        MDNS.update();
    }

    void Webserver::apiSendJSON(int status) 
    {
        String json;
        serializeJson(m_jsonDoc, json);
        m_server.send(status, "application/json", json);
    }

    void Webserver::apiSendOK()
    {
        m_jsonDoc.clear();
        m_jsonDoc["status"] = "ok";
        apiSendJSON(200);
    }

    void Webserver::apiSendError(String message)
    {
        m_jsonDoc.clear();
        m_jsonDoc["error"] = message;
        apiSendJSON(400);
    }

    void Webserver::apiGetStatus()
    {
        m_jsonDoc.clear();
        m_jsonDoc["status"] = String("OK");
        apiSendJSON(200);
    }

    void Webserver::apiSetDateTime()
    {
        uint16_t year = 0;
        uint8_t month = 0;
        uint8_t day = 0;
        uint8_t hour = 24;
        uint8_t minute = 60;
        if (m_server.hasArg("day"))
        {
            day = (uint8_t) m_server.arg("day").toInt();
        }
        if (m_server.hasArg("month"))
        {
            month = (uint8_t) m_server.arg("month").toInt();
        }
        if (m_server.hasArg("year"))
        {
            year = (uint16_t) m_server.arg("year").toInt();
        }
        if (m_server.hasArg("hour"))
        {
            hour = (uint8_t) m_server.arg("hour").toInt();
        }
        if (m_server.hasArg("minute"))
        {
            minute = (uint8_t) m_server.arg("minute").toInt();
        }
        rtc.setDateTime(year, month, day, hour, minute);
        apiSendOK();
    }


    void Webserver::apiSetBrightness()
    {
        if (m_server.hasArg("type"))
        {
            String arg = m_server.arg("type");
            if(arg == "fixed") 
            {
                config.brightnessMode = fixedBrightness;
            }
            else if (arg == "time")
            {
                config.brightnessMode = timeBrightness;
            }
            else 
            {
                apiSendError("Unknown type, valid options: 'fixed', 'ldr', 'time'");
                return;
            }
        }

        if (m_server.hasArg("min"))
        {
            config.minBrightness = m_server.arg("min").toInt();
        }

        if (m_server.hasArg("max"))
        {
            config.maxBrightness = m_server.arg("max").toInt();
        }

        if (m_server.hasArg("startHour")) {
            config.brightnessStartHour = m_server.arg("startHour").toInt();
        }

        if (m_server.hasArg("endHour")) {
            config.brightnessEndHour = m_server.arg("endHour").toInt();
        }

        saveConfiguration();
        apiSendOK();  
    }


    Webserver webserver;
}

