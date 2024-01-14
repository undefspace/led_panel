#pragma once

#include "tasks/http.h"
#include "wifi_creds.h"

extern render_task_notification_t _media_response_parse(cJSON* root);
static const http_request_def_t media_request = {
    .log_tag = "media",
    .interval_us = 1 * 1000 * 1000,
    .url = "http://undef:8123/api/states/media_player.mpd",
    .auth = "Bearer "HASS_KEY,
    .use_tls = false,
    .prepare_url = NULL,
    .parse_response = _media_response_parse,
};
