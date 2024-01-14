#include "tasks/render.h"
#include "overall_config.h"
#include "olive.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include "fonts/big_clock.h"
#include <leddisplay.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <math.h>

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

// images
#include "images/snowflake.h"

#define TAG "render_task"

// renderer status
QueueHandle_t render_task_queue;
uint8_t connected_to_wifi;
uint64_t frame_delta;
uint8_t currently_snowing;
render_snowflake_t snowflakes[SNOWFLAKE_CNT];

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
                currently_snowing = wp.snow;
                #undef wp
                break;
            // door ring
            case rt_notif_door_ring:
                ESP_LOGI(TAG, "rt_notif_door_ring");
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
                free(media_state.track);
                free(media_state.album);
                free(media_state.artist);
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
                ESP_LOGI(TAG, "rt_notif_co2_ppm");
                co2_ppm_buf = notification.u.ppm_buffer;
                break;
            // indoor temp
            case rt_notif_temp:
                ESP_LOGI(TAG, "rt_notif_temp: %f", notification.u.indoor_temp);
                break;
            // IR button
            case rt_notif_ir:
                ESP_LOGI(TAG, "rt_notif_ir: %s", ir_button_t_to_str(notification.u.ir_button));
                break;
        }
    }
}

void render_snowflake(Olivec_Canvas canvas, render_snowflake_t* sf) {
    float x_offs = 7.0f * sf->mass * sin(sf->phase + ((float)(esp_timer_get_time() / 10000ll) / 50.0f));
    olivec_image_draw(canvas, sf->x + x_offs, sf->y, snowflake, sf->alpha);

    sf->y += sf->mass * 10.0f * ((float)frame_delta / 1000000.0f);
    if(sf->y >= PANEL_HEIGHT + snowflake.height)
        render_snowflake_new(sf);
}

void render_snowflake_new(render_snowflake_t* sf) {
    sf->mass = 0.5f + (0.5f * ((float)rand() / RAND_MAX));
    sf->phase = M_TWOPI * (float)rand() / RAND_MAX;
    sf->x = (int64_t)PANEL_WIDTH * (int64_t)rand() / (int64_t)RAND_MAX;
    sf->y = -(int)snowflake.height;
    sf->alpha = rand() & 0xff;
}

void render_task(void* ignored) {
    // create queue
    render_task_queue = xQueueCreate(RENDER_QUEUE_LEN, sizeof(render_task_notification_t));

    // create initial snowflakes
    for(int i = 0; i < SNOWFLAKE_CNT; i++)
        render_snowflake_new(snowflakes + i);

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
            middle_section_target = (middle_section_target + PANEL_WIDTH) % (PANEL_WIDTH * MIDDLE_CNT);
        }
        float delta = middle_section_target - middle_section_offs; // the fact that delta keeps getting smaller gives the animation some smoothness
        float inc = delta * ((float)frame_delta / (MIDDLE_SECT_TRANS_MS * 1000.0f));
        middle_section_offs += inc;
        if(fabs(middle_section_offs - middle_section_target) < 1)
            middle_section_offs = middle_section_target;

        // render moving elements
        ui_element_draw(middle_canvas, weather);
        ui_element_draw(middle_canvas, co2);
        olivec_sprite_copy(canvas, -middle_section_offs, 31, middle_canvas.width, middle_canvas.height, middle_canvas);

        // render snowflakes
        if(currently_snowing)
            for(int i = 0; i < SNOWFLAKE_CNT; i++)
                render_snowflake(canvas, snowflakes + i);

        // darken the screen and draw a Wi-Fi icon if not connected to WiFi yet
        if(!connected_to_wifi) {
            olivec_rect(canvas, 0, 0, PANEL_WIDTH, PANEL_HEIGHT, 0xd0000000);
            olivec_text(canvas, "W", 27, 25, big_clock, 0xffffffff);
        }
        
        // update display
        for(uint8_t y = 0; y < PANEL_HEIGHT; y++) {
            for(uint8_t x = 0; x < PANEL_WIDTH; x++) {
                uint32_t rgb = buffer[(y * PANEL_WIDTH) + x];
                leddisplay_frame.yx[y][x][0] = rgb >> 16;
                leddisplay_frame.yx[y][x][1] = rgb >> 8;
                leddisplay_frame.yx[y][x][2] = rgb;
            }
        }
        leddisplay_frame_update(&leddisplay_frame);
        
        // yield
        vTaskDelay(10 / portTICK_PERIOD_MS);

        frame_delta = esp_timer_get_time() - frame_start;
        // ESP_LOGI(TAG, "delta=%lldus, fps=%.1f", frame_delta, 1000000.0f / frame_delta);
    }
}
