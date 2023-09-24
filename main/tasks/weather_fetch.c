#include "tasks/weather_fetch.h"
#include "overall_config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_http_client.h>
#include <cJSON.h>
#include "tasks/render.h"
#include <esp_log.h>

#define TAG "weather_fetch_task"

extern const char root_cert[] asm("_binary_usertrust_rsa_ca_pem_start");

esp_err_t _weather_http_event_handler(esp_http_client_event_t *evt) {
    static int output_len;
    if(evt->event_id == HTTP_EVENT_ON_CONNECTED) {
        output_len = 0;
    } else if(evt->event_id == HTTP_EVENT_ON_DATA) {
        memcpy(evt->user_data + output_len, evt->data, evt->data_len);
        output_len += evt->data_len;
    }
    return ESP_OK;
}

void weather_fetch_task(void* _ignored) {
    // configure HTTP client
    char json[4096] = {0};
    const esp_http_client_config_t config = {
        .host = "api.openweathermap.org",
        .path = "/data/2.5/weather",
        .query = "q="WEATHER_LOCATION"&appid="WEATHER_KEY"&units=metric",
        .transport_type = HTTP_TRANSPORT_OVER_SSL,
        .cert_pem = root_cert,
        .cert_len = 0, // null-terminated
        .user_data = json,
        .event_handler = _weather_http_event_handler,
    };

    while(1) {
        // fetch weather JSON
        esp_http_client_handle_t client = esp_http_client_init(&config);
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
        int icon_id;
        char time_of_day;
        cJSON* root = cJSON_Parse(json);
        cJSON* weather_list = cJSON_GetObjectItem(root, "weather");
        cJSON* weather = cJSON_GetArrayItem(weather_list, 0);
        cJSON* main = cJSON_GetObjectItem(root, "main");
        float temp = cJSON_GetNumberValue(cJSON_GetObjectItem(main, WEATHER_USE_FEELS_LIKE ? "feels_like" : "temp"));
        sscanf(cJSON_GetStringValue(cJSON_GetObjectItem(weather, "icon")), "%02d%c", &icon_id, &time_of_day);

        // transform icon codes
        render_task_notification_t notification = {
            .type = rt_notif_weather,
            .u.weather.temperature = temp,
        };
        #define wp notification.u.weather
        switch(icon_id) {
            case 1:
                wp.sun = 1;
                break;
            case 2:
                wp.sun = 1;
                wp.cloud = 1;
                break;
            case 3:
            case 4:
                wp.cloud = 1;
                break;
            case 9:
                wp.cloud = 1;
                wp.rain = 1;
                break;
            case 10:
                wp.sun = 1;
                wp.cloud = 1;
                wp.rain = 1;
                break;
            case 11:
                wp.cloud = 1;
                wp.thunder = 1;
                break;
            case 13:
                wp.snow = 1;
                break;
            case 50:
                wp.mist = 1;
                break;
        }
        #undef wp

        // send notification to renderer process
        xQueueSend(render_task_queue, &notification, 0);

        // fetch again in 10min
        esp_http_client_close(client);
        esp_http_client_cleanup(client);
        vTaskDelay(10 * 60 * 1000 / portTICK_PERIOD_MS);
    }
}
