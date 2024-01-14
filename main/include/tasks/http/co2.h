#pragma once

#include "tasks/http.h"
#include "wifi_creds.h"
#include <time.h>

time_t timegm(struct tm* tm);

extern render_task_notification_t _co2_response_parse(cJSON* root);
extern void _co2_url_prepare(char* buf, const char* template);
static const http_request_def_t co2_request = {
    .log_tag = "co2",
    .interval_us = 30 * 1000 * 1000,
    .url = "http://undef:8123/api/history/period/%s?filter_entity_id=sensor.co2&minimal_response&no_attributes",
    .auth = "Bearer "HASS_KEY,
    .use_tls = false,
    .prepare_url = _co2_url_prepare,
    .parse_response = _co2_response_parse,
};
