#include "tasks/render.h"
#include "overall_config.h"
#include "olive.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "fonts/big_clock.h"
#include <leddisplay.h>
#include <esp_log.h>
#include <esp_timer.h>

// elements
#include "ui_element.h"
#include "elements/analog_clock.h"
#include "elements/binary_clock.h"
#include "elements/decimal_clock.h"
#include "elements/hex_clock.h"
#include "elements/weather.h"
#include "elements/co2.h"
#include "elements/spectrum.h"
#include "elements/media.h"

#define TAG "render_task"

// renderer status
QueueHandle_t render_task_queue;
uint8_t connected_to_wifi;
uint8_t brightness = 255;
uint64_t frame_delta;

// framebuffers
leddisplay_frame_t leddisplay_frame;
static uint32_t buffer[PANEL_WIDTH * PANEL_HEIGHT];
uint32_t middle_buffer[PANEL_WIDTH * MIDDLE_CNT * 14];

// middle section (moving elements)
float middle_section_offs = 0;
int middle_section_target = 0;
int64_t middle_section_latch = 0;

void _render_task_process_notifications(void) { 
    render_task_notification_t notification;
    while(xQueueReceive(render_task_queue, &notification, 0)) {
        switch(notification.type) {
            // WiFi status
            case rt_notif_wifi_status:
                ESP_LOGI(TAG, "rt_notif_wifi_status: %d", connected_to_wifi);
                connected_to_wifi = notification.u.wifi_connected;
                break;
            // weather update
            case rt_notif_weather:
                #define wp notification.u.weather
                ESP_LOGI(TAG, "rt_notif_weather: sun=%d, cloud=%d, rain=%d, thunder=%d, snow=%d, temperature=%02.1fC",
                    wp.sun, wp.cloud, wp.rain, wp.thunder, wp.snow, wp.temperature);
                weather_status = (wp.sun) | (wp.cloud << 1) | (wp.rain << 2) | (wp.thunder << 3)
                    | (wp.snow << 4) | (wp.mist << 5);
                weather_temp = wp.temperature;
                #undef wp
                break;
            // door ring
            case rt_notif_door_ring:
                ESP_LOGI(TAG, "rt_notif_door_ring");
                break;
            // brightness
            case rt_notif_brightness:
                ESP_LOGI(TAG, "rt_notif_brightness: %d", notification.u.brightness);
                brightness = notification.u.brightness;
                break;
            // fft buffer
            case rt_notif_fft:
                // don't log (too frequent)
                break;
            // media info
            case rt_notif_media_info:
                #define mp notification.u.media_info
                // log
                ESP_LOGI(TAG, "rt_notif_media_info: track=\"%s\", album=\"%s\", artist=\"%s\", length=%ums, playing=%d, pos=%u",
                    mp.track, mp.album, mp.artist, mp.length, mp.playing, mp.pos);
                // free previous strings
                free(mp.track);
                free(mp.album);
                free(mp.artist);
                // set new data
                media_state = (media_state_t){
                    .track = mp.track,
                    .album = mp.album,
                    .artist = mp.artist,
                    .length = mp.length,
                    .playing = mp.playing,
                    .pos = mp.pos,
                };
                #undef mp
                break;
            // CO2 ppm
            case rt_notif_co2_ppm:
                ESP_LOGI(TAG, "rt_notif_co2_ppm: %d", notification.u.co2_ppm);
                co2_ppm = notification.u.co2_ppm;
                break;
            // indoor temp
            case rt_notif_temp:
                ESP_LOGI(TAG, "rt_notif_temp: %f", notification.u.indoor_temp);
                break;
        }
    }
}

void render_task(void* ignored) {
    // create queue
    render_task_queue = xQueueCreate(RENDER_QUEUE_LEN, sizeof(render_task_notification_t));

    while(1) {
        uint64_t frame_start = esp_timer_get_time();

        // process incoming notifications
        _render_task_process_notifications();

        // clear buffers
        leddisplay_frame_clear(&leddisplay_frame);
        Olivec_Canvas canvas = olivec_canvas(buffer, PANEL_WIDTH, PANEL_HEIGHT, PANEL_WIDTH);
        Olivec_Canvas middle_canvas = olivec_canvas(middle_buffer, PANEL_WIDTH * MIDDLE_CNT, 14, PANEL_WIDTH * MIDDLE_CNT);
        olivec_fill(canvas, 0xff000000);
        olivec_fill(middle_canvas, 0xff000000);

        // render persistent elements
        ui_element_draw(canvas, decimal_clock);
        ui_element_draw(canvas, hex_clock);
        ui_element_draw(canvas, binary_clock);
        ui_element_draw(canvas, analog_clock);
        ui_element_draw(canvas, spectrum);
        ui_element_draw(canvas, media);

        // recalculate moving element position
        if(esp_timer_get_time() - middle_section_latch > (MIDDLE_SECT_HOLD_MS * 1000)) {
            middle_section_latch = esp_timer_get_time();
            middle_section_target = (middle_section_target + 64) % (64 * MIDDLE_CNT);
        }
        float delta = middle_section_target - middle_section_offs; // the fact that delta keeps getting smaller gives the animation some smoothness
        float inc = delta * ((float)frame_delta / (MIDDLE_SECT_TRANS_MS * 1000.0f));
        middle_section_offs += inc;

        // render moving elements
        ui_element_draw(middle_canvas, weather);
        ui_element_draw(middle_canvas, co2);
        olivec_sprite_copy(canvas, -middle_section_offs, 31, middle_canvas.width, middle_canvas.height, middle_canvas);

        // darken the screen and draw a Wi-Fi icon if not connected to WiFi yet
        if(!connected_to_wifi) {
            olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, 0xd0000000);
            olivec_text(canvas, "W", 27, 25, big_clock, 0xffffffff);
        }

        // apply brightness
        olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, (255 - brightness) << 24);
        
        // update display
        for(uint8_t y = 0; y < PANEL_HEIGHT; y++) {
            for(uint8_t x = 0; x < PANEL_WIDTH; x++) {
                uint32_t rgb = buffer[(y * PANEL_WIDTH) + x];
                uint8_t r = rgb >> 16;
                uint8_t g = rgb >> 8;
                uint8_t b = rgb;
                leddisplay_frame_xy_rgb(&leddisplay_frame, x, y, r, g, b);
            }
        }
        leddisplay_frame_update(&leddisplay_frame);
        
        // yield
        vTaskDelay(10 / portTICK_PERIOD_MS);

        frame_delta = esp_timer_get_time() - frame_start;
    }
}
