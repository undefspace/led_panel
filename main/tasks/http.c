#include "tasks/http.h"
#include "overall_config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_http_client.h>
#include <cJSON.h>
#include "tasks/render.h"
#include <esp_log.h>
#include "tasks/http/co2.h"
#include "tasks/http/temp.h"
#include "tasks/http/weather.h"
#include <esp_timer.h>

#define TAG "http_task"

extern const char root_cert[] asm("_binary_usertrust_rsa_ca_pem_start");

http_request_def_t http_defs[] = {
    co2_request,
    weather_request,
};

esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static int output_len;
    if(evt->event_id == HTTP_EVENT_ON_CONNECTED) {
        output_len = 0;
    } else if(evt->event_id == HTTP_EVENT_ON_DATA) {
        memcpy(evt->user_data + output_len, evt->data, evt->data_len);
        output_len += evt->data_len;
    }
    return ESP_OK;
}

int _http_perform_request(http_request_def_t req) {
    // initial configuration
    char json[4096] = {0};
    esp_http_client_config_t config = {
        .url = req.url,
        .event_handler = _http_event_handler,
        .user_data = json,
    };
    if(req.use_tls) {
        config.cert_pem = root_cert;
        config.cert_len = 0; // null-terminated
        config.transport_type = HTTP_TRANSPORT_OVER_SSL;
    }

    // create client
    esp_http_client_handle_t client = esp_http_client_init(&config);
    if(req.auth)
        esp_http_client_set_header(client, "Authorization", req.auth);

    // perform request
    esp_err_t err = esp_http_client_perform(client);
    if(err == ESP_OK) {
        ESP_LOGI(TAG, "%s: HTTP GET Status = %d, content_length = %lld",
                req.log_tag,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "%s: HTTP GET request failed: %s", req.log_tag, esp_err_to_name(err));
        ESP_ERROR_CHECK(esp_http_client_cleanup(client));
        return 0;
    }

    // parse json and send notification
    cJSON* root = cJSON_Parse(json);
    render_task_notification_t notif = req.parse_response(root);
    xQueueSend(render_task_queue, &notif, 0);

    // clean up
    ESP_ERROR_CHECK(esp_http_client_cleanup(client));
    cJSON_Delete(root);

    return 1;
}

void http_task(void* _ignored) {
    // set initial offsets so that requests get performed immediately
    for(int i = 0; i < sizeof(http_defs) / sizeof(*http_defs); i++) {
        http_request_def_t* req = &http_defs[i];
        req->last_performed = -req->interval_us;
    }

    while(1) {
        // perform requests
        for(int i = 0; i < sizeof(http_defs) / sizeof(*http_defs); i++) {
            http_request_def_t* req = &http_defs[i];
            if(esp_timer_get_time() - req->last_performed >= req->interval_us) {
                if(_http_perform_request(*req))
                    req->last_performed = esp_timer_get_time(); // successful
                else
                    req->last_performed = esp_timer_get_time() - req->interval_us + 3000000; // unsuccessful (retry after 3s)
            }
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
