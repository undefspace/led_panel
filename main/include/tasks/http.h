#pragma once

#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <tasks/render.h>
#include <cJSON.h>

typedef struct {
    const char* log_tag;
    int64_t interval_us, last_performed;
    const char* url;
    const char* auth;
    bool use_tls;
    render_task_notification_t (*parse_response)(cJSON* root);
} http_request_def_t;

void http_task(void*);
