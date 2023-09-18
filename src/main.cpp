#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FastLED.h>
#include <WiFi.h>
#include <time.h>
#include "config.h"
#include "wifi_creds.h"
#include "panel_loop.h"

MatrixPanel_I2S_DMA* panel = nullptr;
static uint32_t main_panel_buffer[PANEL_WIDTH * PANEL_HEIGHT];
CRGB strip[STRIP_WIDTH];

void setup(void) {
    Serial.begin(115200);
    delay(1000);

    // connect to to Wi-Fi
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while(WiFi.status() != WL_CONNECTED);
    Serial.println("Connected to Wi-Fi");

    // set up NTP
    configTime(NTP_OFFSET, NTP_DAYLIGHT_OFFSET, NTP_SERVER);

    // set up panel
    HUB75_I2S_CFG mx_config;
    mx_config.mx_height = PANEL_HEIGHT;
    mx_config.chain_length = 1;
    mx_config.gpio.e = PANEL_PIN_E;
    mx_config.gpio.oe = PANEL_PIN_OE;
    mx_config.i2sspeed = HUB75_I2S_CFG::HZ_20M;
    panel = new MatrixPanel_I2S_DMA(mx_config);
    panel->begin();
    panel->setBrightness8(255);

    // set up strip
    FastLED.addLeds<WS2812B, STRIP_PIN_D>(strip, STRIP_WIDTH);
}

void loop(void) {
    uint64_t frame_start = micros();

    // draw persistent elements
    panel_loop(main_panel_buffer);

    // transfer olivec canvas to DMA buffer
    for(int y = 0; y < PANEL_HEIGHT; y++) {
        for(int x = 0; x < PANEL_WIDTH; x++) {
            uint32_t color = main_panel_buffer[(y * PANEL_WIDTH) + x];
            uint8_t r = (color >> 16) & 0xff;
            uint8_t g = (color >> 8) & 0xff;
            uint8_t b = color & 0xff;
            panel->drawPixelRGB888(x, y, r, g, b);
        }
    }

    // maintain refresh rate
    uint64_t delta = micros() - frame_start;
    if(delta < PANEL_DELTA_TARGET)
        delayMicroseconds(PANEL_DELTA_TARGET - delta);
}
