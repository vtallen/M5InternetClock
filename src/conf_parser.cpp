#include <M5Core2.h>
#include <string>

#include <stocks/stocks.h>

#include <vector>

namespace Conf {
// WiFi Credentials
char *SSID{};
char *PASSWORD{};
int *TIME_OFFSET{};

// Stock API for fetching prices
char *ALPHA_VANTAGE_KEY{};

char *getSSID() {
  assert(SSID);
  return SSID;
}

char *getPASSWORD() {
  assert(PASSWORD);
  return PASSWORD;
}

char *getALPHA_VANTAGE_KEY() {
  assert(ALPHA_VANTAGE_KEY);
  return ALPHA_VANTAGE_KEY;
}

int *getTIME_OFFSET() {
  assert(TIME_OFFSET);
  return TIME_OFFSET;
}

void parseConfigLine(String &value) {
  int delimiterIndex;
  String parameter;

  delimiterIndex = value.indexOf("=");

  parameter = value.substring(0, delimiterIndex);
  value = value.substring(delimiterIndex + 1);

  parameter.trim();
  value.trim();

  // Make a C string to hold the SSID, copy the SSID into it, then assign the
  // temp pointer to the pointer defined at the top of this file The setup
  // function can then access this pointer to configure the WIFI connection
  if (parameter == "SSID") {
    char *value_ptr = new char[strlen(value.c_str()) + 1];
    strcpy(value_ptr, value.c_str());

    SSID = value_ptr;
  } else if (parameter == "PASSWORD") {
    char *value_ptr = new char[strlen(value.c_str()) + 1];
    strcpy(value_ptr, value.c_str());

    PASSWORD = value_ptr;
  } else if (parameter == "ALPHA_VANTAGE_KEY") {
    char *value_ptr = new char[strlen(value.c_str()) + 1];
    strcpy(value_ptr, value.c_str());

    ALPHA_VANTAGE_KEY = value_ptr;
  } else if (parameter == "TIME_OFFSET_UTC_SECONDS") {
    int offset = std::stoi(value.c_str());
    TIME_OFFSET = &offset;
  }
}

void loadConfig() {
  File file = SD.open("/M5Clock.txt");
  if (file) {
    while (file.available()) {
      String c = file.readStringUntil('\n');
      parseConfigLine(c);
    }
    file.close();
    // causes stocks.cpp to call getALPHA_VANTAGE_KEY
    Stocks::init();
  } else {
    Serial.println("Failed to open WidgetClock.txt.");
  }
}

} // namespace Conf