#include <Arduino.h>
#include <M5GFX.h>

namespace Theme {
    constexpr u_int16_t BACKGROUND_COLOR = 0xAE29;
    constexpr u_int16_t FOREGROUND_COLOR = 0x3A05;

    const lgfx::v1::RLEfont CLOCK_FONT = fonts::Font7;
    const lgfx::v1::GFXfont TEXT_FONT = fonts::Orbitron_Light_24;
}