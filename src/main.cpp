#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <FastLED.h>
#include <WiFi.h>
#include <time.h>
#include "config.h"
#include "wifi_creds.h"
#include "render_task.h"
#include "weather_fetch_task.h"

MatrixPanel_I2S_DMA* panel = nullptr;
static uint32_t main_panel_buffer[PANEL_WIDTH * PANEL_HEIGHT];
CRGB strip[STRIP_WIDTH];

void setup(void) {
    Serial.begin(115200);
    delay(1000);

    // connect to to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASS);

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

    // create tasks
    xTaskCreate(render_task, "led_panel", 8192, main_panel_buffer, 10, NULL);
    xTaskCreate(weather_fetch_task, "weather", 8192, NULL, 10, NULL);

    // set up strip
    FastLED.addLeds<WS2812B, STRIP_PIN_D>(strip, STRIP_WIDTH);
}

// the render task calls this function to set a pixel
// we need this wrapper because the rest of the project is written in C
void dma_draw_rgb(uint8_t x, uint8_t y, uint32_t color) {
    uint8_t r = (color >> 16) & 0xff;
    uint8_t g = (color >> 8) & 0xff;
    uint8_t b = color & 0xff;
    panel->drawPixelRGB888(x, y, r, g, b);
}


void loop(void) {
    // send Wi-Fi status updates
    render_task_notification_t notification = {
        .type = rt_notif_wifi_status,
        .u = {
            .wifi_connected = WiFi.status() == WL_CONNECTED,
        }
    };
    xQueueSend(render_task_queue, &notification, 0);

    vTaskDelay(500 / portTICK_PERIOD_MS);
}
