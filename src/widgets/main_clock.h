#include <Arduino.h>
#include <M5Core2.h>

namespace Main_Clock {
    void drawClock(RTC_TimeTypeDef &TimeStruct, M5Canvas &canvas, int x, int y, float size, bool is_12_hr = true);
}