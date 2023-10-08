#pragma once

// NTP
#define NTP_SERVER  "pool.ntp.org"
#define TZ          "GMT-3"

// LED panel
#define PANEL_WIDTH  64
#define PANEL_HEIGHT 64
#define PANEL_PIN_E  18
#define PANEL_PIN_OE 22
#define PANEL_RATE   144
#define PANEL_DELTA_TARGET (1000000UL / PANEL_RATE)

// LED strip
#define STRIP_PIN_D 21
#define STRIP_WIDTH 300

// moving elements (middle section)
#define MIDDLE_SECT_HOLD_MS  5000
#define MIDDLE_SECT_TRANS_MS 300
#define MIDDLE_CNT           2

// spectrum
#define FFT_SIZE               512
#define SPECTRUM_FPS           60

// misc
#define RENDER_QUEUE_LEN       8
#define WEATHER_LOCATION       "Taganskiy,Moscow,RU"
#define WEATHER_USE_FEELS_LIKE true
#define IR_RX_GPIO_NUM         33

// credentials
#include "wifi_creds.h"
