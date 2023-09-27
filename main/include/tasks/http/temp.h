#pragma once

#include "tasks/http.h"
#include "wifi_creds.h"

extern render_task_notification_t _temp_response_parse(cJSON* root);
static const http_request_def_t temp_request = {
    .log_tag = "temp",
    .interval_us = 30 * 1000 * 1000,
    .url = "http://undef:8123/api/states/sensor.temperature",
    .auth = "bearer "HASS_KEY,
    .use_tls = false,
    .parse_response = _temp_response_parse,
};
