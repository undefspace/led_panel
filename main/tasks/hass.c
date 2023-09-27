#include "tasks/hass.h"
#include "overall_config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_http_client.h>
#include <cJSON.h>
#include "tasks/render.h"
#include <esp_log.h>

#define TAG "hass_task"

esp_err_t _hass_http_event_handler(esp_http_client_event_t *evt) {
    static int output_len;
    if(evt->event_id == HTTP_EVENT_ON_CONNECTED) {
        output_len = 0;
    } else if(evt->event_id == HTTP_EVENT_ON_DATA) {
        memcpy(evt->user_data + output_len, evt->data, evt->data_len);
        output_len += evt->data_len;
    }
    return ESP_OK;
}

void hass_task(void* _ignored) {
    // configure HTTP client
    char json[512] = {0};
    const esp_http_client_config_t config = {
        .host = "undef",
        .port = 8123,
        .path = "/api/states/sensor.co2",
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .user_data = json,
        .event_handler = _hass_http_event_handler,
    };

    while(1) {
        // fetch ppm JSON
        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_http_client_set_header(client, "Authorization", "Bearer "HASS_KEY);
        esp_err_t err = esp_http_client_perform(client);
        if(err == ESP_OK) {
            ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %d",
                    esp_http_client_get_status_code(client),
                    esp_http_client_get_content_length(client));
        } else {
            ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
            // retry after 3s
            esp_http_client_close(client);
            esp_http_client_cleanup(client);
            vTaskDelay(3000 / portTICK_PERIOD_MS);
            continue;
        }

        // parse JSON
        cJSON* root = cJSON_Parse(json);
        int ppm = atoi(cJSON_GetStringValue(cJSON_GetObjectItem(root, "state")));
        cJSON_Delete(root);

        // send notification to renderer process
        render_task_notification_t notification = {
            .type = rt_notif_co2_ppm,
            .u.co2_ppm = ppm,
        };
        xQueueSend(render_task_queue, &notification, 0);

        // fetch again in 30sec
        esp_http_client_cleanup(client);
        vTaskDelay(30 * 1000 / portTICK_PERIOD_MS);
    }
}
