#pragma once

#include "tasks/http.h"
#include "overall_config.h"

extern render_task_notification_t _weather_response_parse(cJSON* root);
static const http_request_def_t weather_request = {
    .log_tag = "weather",
    .interval_us = 10 * 60 * 1000 * 1000,
    .url = "https://api.openweathermap.org/data/2.5/weather?q="WEATHER_LOCATION"&appid="WEATHER_KEY"&units=metric",
    .auth = NULL,
    .use_tls = true,
    .parse_response = _weather_response_parse,
};
