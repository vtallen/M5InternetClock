#include <Arduino.h>
#include <M5Core2.h>

namespace Main_Clock {
    void setTimeStruct(RTC_TimeTypeDef *TimeStructPtr);
    void setDateStruct(RTC_DateTypeDef *DateStructPtr);
    
    void drawClock(M5Canvas canvas, int x, int y, float size, bool is_12_hr = true);
}