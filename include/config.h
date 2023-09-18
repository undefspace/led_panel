// NTP
#define NTP_SERVER          "pool.ntp.org"
#define NTP_OFFSET          (3600 * 3) // GMT+3
#define NTP_DAYLIGHT_OFFSET 0

// LED panel
#define PANEL_WIDTH  64
#define PANEL_HEIGHT 64
#define PANEL_PIN_E  18
#define PANEL_PIN_OE 22
#define PANEL_RATE   144
#define PANEL_DELTA_TARGET (1000000UL / PANEL_RATE)

// LED strip
#define STRIP_PIN_D 21
#define STRIP_WIDTH 240