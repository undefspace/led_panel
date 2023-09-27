#pragma once

#include "tasks/http.h"
#include "wifi_creds.h"

extern render_task_notification_t _co2_response_parse(cJSON* root);
static const http_request_def_t co2_request = {
    .log_tag = "co2",
    .interval_us = 30 * 1000 * 1000,
    .url = "http://undef:8123/api/states/sensor.co2",
    .auth = "Bearer "HASS_KEY,
    .use_tls = false,
    .parse_response = _co2_response_parse,
};
