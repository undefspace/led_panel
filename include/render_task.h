#pragma once
#ifdef __cplusplus
extern "C" {
#endif

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
        bool wifi_connected;
        struct {
            int8_t temperature;
            bool sun, cloud, rain, thunder, snow;
        } weather;
    } u;
} render_task_notification_t;

extern QueueHandle_t render_task_queue;

// defined in main.cpp
void dma_draw_rgb(uint8_t x, uint8_t y, uint32_t color);

void render_task(void* buffer);

#ifdef __cplusplus
}
#endif
