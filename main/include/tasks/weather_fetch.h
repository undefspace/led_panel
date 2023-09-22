#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// void weather_fetch_task_init(void);
void weather_fetch_task(void*);

#ifdef __cplusplus
}
#endif
