#pragma once

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

// void weather_fetch_task_init(void);
void weather_fetch_task(void*);