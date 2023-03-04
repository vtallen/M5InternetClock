#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <M5Core2.h>
#include <M5GFX.h>
#include <WiFi.h>

#include <stocks/stocks.h>
#include <theme.h>

namespace Main_Clock {
constexpr u_int16_t BACKGROUND_COLOR = 0xAE29;
constexpr u_int16_t FOREGROUND_COLOR = 0x3A05;

RTC_TimeTypeDef *TimeStruct;
RTC_DateTypeDef *DateStruct;

M5Canvas *canvas;

void setTimeStruct(RTC_TimeTypeDef *TimeStructPtr) {
  assert(TimeStructPtr);
  TimeStruct = TimeStructPtr;
}

void setDateStruct(RTC_DateTypeDef *DateStructPtr) {
  assert(DateStructPtr);
  DateStruct = DateStructPtr;
}

void setCanvas(M5Canvas *CanvasPtr) {
  assert(CanvasPtr);
  canvas = CanvasPtr;
}

// Exactly what it sounds like
uint8_t convertToTwelveHour(u_int8_t hour) {
  switch (hour) {
  case 13:
    return 1;
  case 14:
    return 2;
  case 15:
    return 3;
  case 16:
    return 4;
  case 17:
    return 5;
  case 18:
    return 6;
  case 19:
    return 7;
  case 20:
    return 8;
  case 21:
    return 9;
  case 22:
    return 10;
  case 23:
    return 11;
  case 24:
    return 12;
  default:
    return hour;
  }
}

// Formats the time so that it has a constant width
String addLeadingZero(uint8_t time) {
  if (time < 10) {
    return "0" + String(time);
  } else {
    return String(time);
  }
}

// Converts the numeric week day into a three letter string.
String getWeekDay(uint8_t weekday) {
  switch (weekday) {
  case 1:
    return "MON";
  case 2:
    return "TUE";
  case 3:
    return "WED";
  case 4:
    return "THU";
  case 5:
    return "FRI";
  case 6:
    return "SAT";
  case 7:
    return "SUN";
  default:
    return "NOT A VALID DATE";
  }
}

// Converts the numeric month into a three letter string
String getMonth(uint8_t month) {
  switch (month) {
  case 1:
    return "JAN";
  case 2:
    return "FEB";
  case 3:
    return "MAR";
  case 4:
    return "APR";
  case 5:
    return "MAY";
  case 6:
    return "JUN";
  case 7:
    return "JUL";
  case 8:
    return "AUG";
  case 9:
    return "SEP";
  case 10:
    return "OCT";
  case 11:
    return "NOV";
  case 12:
    return "DEC";
  default:
    return "INVALID MONTH NUM";
  }
}

String getOrdinalIndicator(int num) {
  String indicator = "th";
  int lastDigit = num % 10;
  int lastTwoDigits = num % 100;

  if (lastDigit == 1 && lastTwoDigits != 11) {
    indicator = "st";
  } else if (lastDigit == 2 && lastTwoDigits != 12) {
    indicator = "nd";
  } else if (lastDigit == 3 && lastTwoDigits != 13) {
    indicator = "rd";
  }

  return indicator;
}

// Draws the clock on to the canvas, does not update the canvas with the new
// information
void drawClock(int x, int y, float size, bool is_12_hr = true) {
  String hour;
  if (is_12_hr) {
    hour = String(addLeadingZero(convertToTwelveHour(TimeStruct->Hours)));
  } else {
    hour = String(addLeadingZero(TimeStruct->Hours));
  }
  String minute = String(addLeadingZero(TimeStruct->Minutes));
  String second = String(addLeadingZero(TimeStruct->Seconds));

  String time = hour + ":" + minute;

  canvas->setFont(&Theme::CLOCK_FONT);
  canvas->setTextColor(Theme::FOREGROUND_COLOR, Theme::BACKGROUND_COLOR);

  // Draw the hour and minute on the canvas
  canvas->setTextSize(size);
  canvas->setCursor(x, y);
  canvas->drawString(time, x, y);

  // Draw the seconds on the canvas at half the size of the hours. Then scale
  // the offset using the size to place it on the bottom right corner of the
  // hours and minutes.
  canvas->setTextSize(size / 2);
  canvas->setCursor(x + (140 * size), y + (22 * size));
  canvas->drawString((":" + second), x + (140 * size), y + (22 * size));
}

void drawDate(int x, int y, float size) {
  canvas->setFont(&Theme::TEXT_FONT);
  canvas->setTextSize(size);
  canvas->setTextColor(Theme::FOREGROUND_COLOR, Theme::BACKGROUND_COLOR);

  String dateString = getWeekDay(DateStruct->WeekDay) + ", " +
                      getMonth(DateStruct->Month) + " " + DateStruct->Date +
                      getOrdinalIndicator(DateStruct->Date);

  canvas->drawString(dateString, x, y);
}

void drawTemp(int x, int y, float size) {}

void drawStocks(int y, float size) {
  static unsigned long lastUpdateTime{0};
  static unsigned long lastStockSwitch {0};
  static bool isFirstCall {true};
  static int currentStock{0};

  canvas->setFont(&Theme::TEXT_FONT);
  canvas->setTextSize(size);
  canvas->setTextColor(Theme::FOREGROUND_COLOR, Theme::BACKGROUND_COLOR);

  static std::vector<String> STOCK_TICKERS = Stocks::getSTOCK_TICKERS();
  static String text;

  if (isFirstCall) {
    isFirstCall = false;
    Serial.println("Here 1");
    text = STOCK_TICKERS.at(currentStock) + " " +
           Stocks::getQuoteString(STOCK_TICKERS.at(currentStock).c_str());
  }

  if (millis() - lastUpdateTime >= 30000) {
    text = STOCK_TICKERS.at(currentStock) + " " +
           Stocks::getQuoteString(STOCK_TICKERS.at(currentStock).c_str());
    lastUpdateTime = millis(); // update the last update time
  }

  if (millis() - lastStockSwitch >= 60000) {
    Serial.println("Changing stock");
    ++currentStock;
    if (currentStock > (STOCK_TICKERS.size() - 1)) {
      currentStock = 0;
    }

    lastStockSwitch = millis();
  }

  canvas->drawString(text, (320/2) - (canvas->textWidth(text) / 2), y);
}

// This function caused lots of lag... not exactly sure why.
/*
void drawScrollingStocks(int y, float size) {
  static unsigned long lastStockUpdateTime{0};

  static unsigned long lastMoveUpdateTime{0};
  static int x{0};

  canvas->setFont(&Theme::TEXT_FONT);
  canvas->setTextSize(size);
  canvas->setTextColor(Theme::FOREGROUND_COLOR, Theme::BACKGROUND_COLOR);

  static std::vector<String> STOCK_TICKERS = Stocks::getSTOCK_TICKERS();

  static String stock1;
  static String stock2;
  static String stock3;
  static String text;

  if (lastStockUpdateTime == 0) {
    stock1 = STOCK_TICKERS.at(0) + " " +
             Stocks::getQuoteString(STOCK_TICKERS.at(0).c_str());

    stock2 = STOCK_TICKERS.at(1) + " " +
             Stocks::getQuoteString(STOCK_TICKERS.at(1).c_str());

    stock3 = STOCK_TICKERS.at(2) + " " +
             Stocks::getQuoteString(STOCK_TICKERS.at(2).c_str());

    text = stock1 + " - " + stock2 + " - " + stock3;
  }

  unsigned long millisTime = millis();

  if (millisTime - lastStockUpdateTime >= 80000) {
    Serial.println("update");
    stock1 = STOCK_TICKERS.at(0) + " " +
             Stocks::getQuoteString(STOCK_TICKERS.at(0).c_str());

    stock2 = STOCK_TICKERS.at(1) + " " +
             Stocks::getQuoteString(STOCK_TICKERS.at(1).c_str());

    stock3 = STOCK_TICKERS.at(2) + " " +
             Stocks::getQuoteString(STOCK_TICKERS.at(2).c_str());

    text = stock1 + " - " + stock2 + " - " + stock3;

    lastStockUpdateTime = millis(); // update the last update time
  }

  static int offScreenX = (-1 * canvas->textWidth(text)) - 300;

  if (millisTime - lastMoveUpdateTime >= 1000) {
    if (x >= offScreenX) {
      x -= 1;
    } else if (x <= offScreenX) {
      x = 320;
    }
  }

  canvas->drawString(text, x, y);
}
*/

// Draws all of the parts of the widget on to the canvas
void drawWidget() {
  canvas->clear();
  canvas->createSprite(TFT_HEIGHT, TFT_WIDTH);
  canvas->fillSprite(BACKGROUND_COLOR);

  drawClock(30, 60, 1.5);
  drawDate(56, 135, 1);
  drawStocks(200, 1);
}

} // namespace Main_Clock