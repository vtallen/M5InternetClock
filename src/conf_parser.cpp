#include <M5Core2.h>

namespace Conf
{
    char* SSID {};
    char* PASSWORD {};

    char* getSSID() {
        return SSID;
    }

    char* getPASSWORD() {
        return PASSWORD;
    }

    void parseLine(String &value)
    {
        int delimiterIndex;
        String parameter;

        delimiterIndex = value.indexOf("=");
        
        parameter = value.substring(0, delimiterIndex);
        value = value.substring(delimiterIndex + 1);

        parameter.trim();
        value.trim();
    
        Serial.println(value);

        // Make a C string to hold the SSID, copy the SSID into it, then assign the temp pointer to the pointer defined at the top of this file
        // The setup function can then access this pointer to configure the WIFI connection
        if (parameter == "SSID") {
            char* value_ptr = new char[strlen(value.c_str()) + 1];
            strcpy(value_ptr, value.c_str());

            SSID = value_ptr;
        } else if (parameter == "PASSWORD") {
            char* value_ptr = new char[strlen(value.c_str()) + 1];
            strcpy(value_ptr, value.c_str());

            PASSWORD = value_ptr;
        }
    }

    void loadConfig()
    {
        File file = SD.open("/WidgetClock.txt"); 
        if (file) { 
            while (file.available()) {        
                String c = file.readStringUntil('\n'); 
                parseLine(c);            
            }
            file.close(); 
        } else {
            Serial.println("Failed to open file.");
        }
    }

}