#include <Arduino.h>
#include <M5Core2.h>
#include <M5GFX.h>

namespace Main_Clock {
    constexpr u_int16_t BACKGROUND_COLOR = 0xAE29;
    constexpr u_int16_t FOREGROUND_COLOR = 0x3A05;
    const lgfx::v1::RLEfont CLOCK_FONT = fonts::Font7;
    
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

    //Formats the time so that it has a constant width
    String addLeadingZero(uint8_t time) {
        if (time < 10) {
            return "0" + String(time);
        } else {
            return String(time);
        }
    }

    //Draws the clock on to the canvas, does not update the canvas with the new information
    void drawClock(RTC_TimeTypeDef &TimeStruct, M5Canvas &canvas, int x, int y, float size, bool is_12_hr = true) {
        String hour;
        if (is_12_hr) {
            hour = String(addLeadingZero(convertToTwelveHour(TimeStruct.Hours)));
        } else {
            hour = String(addLeadingZero(TimeStruct.Hours));
        }
        String minute = String(addLeadingZero(TimeStruct.Minutes));
        String second = String(addLeadingZero(TimeStruct.Seconds));

        String time = hour + ":" + minute;

        canvas.setCursor(x, y);
        canvas.setTextSize(size);
        canvas.setFont(&CLOCK_FONT);
        canvas.setTextColor(FOREGROUND_COLOR, BACKGROUND_COLOR);

        canvas.drawString(time, x, y);

        canvas.setTextSize(size/2);
        canvas.setCursor(x + (140 * size), y + (22 * size));
        canvas.drawString((":" + second), x + (140 * size), y + (22 * size));
    }

    void drawWidget() {
        
    }
}