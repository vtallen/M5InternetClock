#include <Arduino.h>
#include <M5Core2.h>

#ifndef MAIN_CLOCK_H
#define MAIN_CLOCK_H
namespace Main_Clock {
void setCanvas(M5Canvas *CanvasPtr);
void setTimeStruct(RTC_TimeTypeDef *TimeStructPtr);
void setDateStruct(RTC_DateTypeDef *DateStructPtr);

void drawClock(int x, int y, float size, bool is_12_hr = true);
void drawDate(int x, int y, float size);
void drawTemp(int x, int y, float size);
void drawScrollingStocks(int y, float size);
void drawWidget();
} // namespace Main_Clock
#endif