#pragma once

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

typedef enum {
    rt_notif_wifi_status,
    rt_notif_weather,
    rt_notif_door_ring,
} render_task_notification_type_t;

typedef struct {
    render_task_notification_type_t type;
    union {
        uint8_t wifi_connected;
        struct {
            int8_t temperature;
            uint8_t sun, cloud, rain, thunder, snow;
        } weather;
    } u;
} render_task_notification_t;

extern QueueHandle_t render_task_queue;

void render_task(void* buffer);
