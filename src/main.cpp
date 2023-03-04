#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <M5Core2.h>
#include <M5GFX.h>
#include <NTPClient.h>
#include <SD.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include <conf_parser.h>
#include <stocks/stocks.h>
#include <widgets/main_clock.h>

RTC_TimeTypeDef TimeStruct;
RTC_DateTypeDef DateStruct;

M5GFX display;
M5Canvas canvas(&display);

bool internetConnectionSuccessful{false};

void setup() {
  M5.begin();
  SD.begin(4);

  Conf::loadConfig();

  char *SSID = Conf::getSSID();
  char *PASSWORD = Conf::getPASSWORD();
  int *TIME_OFFSET = Conf::getTIME_OFFSET();

  display.begin();
  display.setTextDatum(top_left);

  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);

  delay(2000); // Wait for wifi to connect

  // Set the time/date using WiFi
  if (WiFi.status() == WL_CONNECTED) {
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);

    timeClient.update();
    timeClient.setTimeOffset(*TIME_OFFSET);

    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime((time_t *)&epochTime);

    TimeStruct.Hours = ptm->tm_hour;
    TimeStruct.Minutes = ptm->tm_min;
    TimeStruct.Seconds = ptm->tm_sec;
    M5.Rtc.SetTime(&TimeStruct);

    // The month is stored as a number from 0-11, therefore we must add 1 to it.
    DateStruct.Month = ptm->tm_mon + 1;
    DateStruct.Date = ptm->tm_mday;
    // The year is returned as the number of years since 1900, so we need to add
    // 1900 to it
    DateStruct.Year = ptm->tm_year + 1900;
    DateStruct.WeekDay = ptm->tm_wday;
    M5.Rtc.SetDate(&DateStruct);

    SD.end();
    internetConnectionSuccessful = true;
  } else {
    Serial.println("WiFi Connection failed! Set time manually.");

    // Temporary offline settinng of the clock so that I can work on this
    // without wifi
    DateStruct.Month = 2;
    DateStruct.Date = 2;
    DateStruct.Year = 2023;

    TimeStruct.Hours = 12;
    TimeStruct.Minutes = 12;
    TimeStruct.Seconds = 12;
  }

  display.begin();
  display.setTextDatum(top_left);

  // Pass the time and date structs to the clock widget so that it does not need
  // them as a parameter
  Main_Clock::setTimeStruct(&TimeStruct);
  Main_Clock::setDateStruct(&DateStruct);
  Main_Clock::setCanvas(&canvas);
  // gets the API key for the alpha vantage API from the conf_parser.cpp
}

void loop() {
  M5.Rtc.GetTime(&TimeStruct);
  M5.Rtc.GetDate(&DateStruct);

  Main_Clock::drawWidget();

  // Update the display
  display.startWrite();
  canvas.pushSprite(0, 0);
  display.endWrite();
}