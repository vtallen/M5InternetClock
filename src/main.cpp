#include <Arduino.h>
#include <M5Core2.h>
#include <SD.h>
#include <M5GFX.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#include <widgets/main_clock.h>
#include <conf_parser.h>

RTC_TimeTypeDef TimeStruct;
RTC_DateTypeDef DateStruct;

M5GFX display;
M5Canvas canvas(&display);

//TODO
/*
- Add a way to set the timezone from that same file
- Add a way to set the time manually, and a variable indicating that this needs to be done.
*/

void setup() {

  char* SSID {Conf::getSSID()};
  char* PASSWORD {Conf::getPASSWORD()};

  M5.begin();
  SD.begin(4);
  
  display.begin();
  display.setTextDatum(top_left);

  Serial.begin(115200);

  Conf::loadConfig();

  WiFi.begin(SSID, PASSWORD);

  delay(2000); // Wait for wifi to connect
  
  // Set the time/date using WiFi
  if (WiFi.status() == WL_CONNECTED) {
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP);

    timeClient.update();
    timeClient.setTimeOffset(-18000); // Set to eastern time, will add the ability to chage this with a config file later

    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime);
    
    TimeStruct.Hours = ptm->tm_hour;
    TimeStruct.Minutes = ptm->tm_min;
    TimeStruct.Seconds = ptm ->tm_sec;
    M5.Rtc.SetTime(&TimeStruct);

    DateStruct.Month = ptm->tm_mon + 1; // The month is stored as a number from 0-11, therefore we must add 1 to it.
    DateStruct.Date = ptm->tm_mday;
    DateStruct.Year = ptm->tm_year + 1900; // The year is returned as the number of years since 1900, so we need to add 1900 to it
    DateStruct.WeekDay = ptm->tm_wday;
    M5.Rtc.SetDate(&DateStruct); 

    SD.end();
  } else {
    Serial.println("WiFi Connection failed! Set time manually.");
  }

  display.begin();
  display.setTextDatum(top_left);

  delete[] SSID;
  delete[] PASSWORD;

  // Pass the time and date structs to the clock widget so that it does not need them as a parameter
  Main_Clock::setTimeStruct(&TimeStruct);
  Main_Clock::setDateStruct(&DateStruct);
}

void loop() {
  constexpr u_int16_t BACKGROUND_COLOR = 0xAE29;
  constexpr u_int16_t FOREGROUND_COLOR = 0x3A05;
  M5.Rtc.GetTime(&TimeStruct);
  M5.Rtc.GetDate(&DateStruct);

  String hour = String(TimeStruct.Hours);
  String minute = String(TimeStruct.Minutes);
  String second = String(TimeStruct.Seconds);

  String time = hour + ":" + minute + ":" + second;
  canvas.clear();
  canvas.createSprite(TFT_HEIGHT, TFT_WIDTH);
  canvas.fillSprite(BACKGROUND_COLOR);
  // canvas.setCursor(0,0);
  // canvas.setTextSize(1);
  // canvas.setFont(&fonts::Font7);
  // canvas.setTextColor(FOREGROUND_COLOR, BACKGROUND_COLOR);
  // canvas.drawString(time, x, y);

  // canvas.setCursor(0, 50);
  // canvas.setFont(&fonts::Orbitron_Light_24);
  // canvas.drawString("Hello World", 0, 50);

  Main_Clock::drawClock(canvas, 30, 40, 1.5);

  // // Only the following process is actually drawn on the panel.
  
  display.startWrite(); 
  canvas.pushSprite(0, 0);
  display.endWrite();

  //delay(1000);
}