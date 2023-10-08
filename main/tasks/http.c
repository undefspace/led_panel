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
#include <sys/param.h>

#define TAG "http_task"
#define HTTP_JSON_SZ 6144

extern const char root_cert[] asm("_binary_usertrust_rsa_ca_pem_start");

http_request_def_t http_defs[] = {
    co2_request,
    weather_request,
};

int http_buf_overflow = 0;
esp_err_t _http_event_handler(esp_http_client_event_t *evt) {
    static int output_len = 0;
    if(evt->event_id == HTTP_EVENT_ON_CONNECTED) {
        output_len = 0;
    } else if(evt->event_id == HTTP_EVENT_ON_DATA) {
        int data_len = MIN(evt->data_len, HTTP_JSON_SZ - output_len);
        memcpy(evt->user_data + output_len, evt->data, data_len);
        output_len += data_len;
        if(data_len <= 0)
            http_buf_overflow = 1;
    }
    return ESP_OK;
}

int _http_perform_request(http_request_def_t req) {
    // prepare url
    char url[256] = {0};
    if(req.prepare_url)
        req.prepare_url(url, req.url);
    else
        strcpy(url, req.url);
    ESP_LOGI(TAG, "%s: url=\"%s\"", req.log_tag, url);

    // initial configuration
    char json[HTTP_JSON_SZ] = {0};
    esp_http_client_config_t config = {
        .url = url,
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
    http_buf_overflow = 0;
    esp_err_t err = esp_http_client_perform(client);
    if(http_buf_overflow) {
        ESP_LOGE(TAG, "%s: HTTP buffer overflow", req.log_tag);
        ESP_ERROR_CHECK(esp_http_client_cleanup(client));
        return 0;
    }
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
    if(root) {
        render_task_notification_t notif = req.parse_response(root);
        if(notif.type > 0) {
            xQueueSend(render_task_queue, &notif, 0);
        } else {
            ESP_ERROR_CHECK(esp_http_client_cleanup(client));
            cJSON_Delete(root);
            return 0;
        }
    }

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
