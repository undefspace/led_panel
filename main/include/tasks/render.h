#pragma once

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

typedef enum {
    rt_notif_wifi_status,
    rt_notif_weather,
    rt_notif_door_ring,
    rt_notif_brightness,
    rt_notif_fft,
} render_task_notification_type_t;

typedef struct {
    render_task_notification_type_t type;
    union {
        uint8_t wifi_connected;
        uint8_t brightness;
        struct {
            float temperature;
            uint8_t sun, cloud, rain, thunder, snow, mist;
        } weather;
        float* fft_buffer;
    } u;
} render_task_notification_t;

extern QueueHandle_t render_task_queue;

void render_task(void* buffer);
