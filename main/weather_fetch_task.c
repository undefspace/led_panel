#include "weather_fetch_task.h"
#include "overall_config.h"
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <esp_http_client.h>
#include <cJSON.h>
#include "render_task.h"
#include <esp_log.h>

#define TAG "weather_fetch_task"

extern const char root_cert[] asm("_binary_isrg_root_x1_pem_start");

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
        .host = "wttr.in",
        .path = "/"WEATHER_LOCATION,
        .query = "format=j2",
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
        if (err == ESP_OK) {
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
        cJSON* condidtions = cJSON_GetObjectItem(root, "current_condition");
        cJSON* condition = cJSON_GetArrayItem(condidtions, 0);
        char* temp_str = cJSON_GetStringValue(cJSON_GetObjectItem(condition,
            WEATHER_USE_FEELS_LIKE ? "FeelsLikeC" : "temp_C"));
        char* condition_code_str = cJSON_GetStringValue(cJSON_GetObjectItem(condition, "weatherCode"));
        int8_t temp = atoi(temp_str);
        uint16_t condition_code = atoi(condition_code_str);

        // transform condition codes
        render_task_notification_t notification = {
            .type = rt_notif_weather,
            .u.weather.temperature = temp,
        };
        #define wp notification.u.weather
        switch(condition_code) {
            case 113: // sunny
                wp.sun = 1;
                break;
            case 116: // partly cloudy
            case 119: // cloudy
                wp.sun = 1;
                wp.cloud = 1;
                break;
            case 122: // overcast
            case 143: // mist
            case 248: // fog
            case 260: // freezing fog
                wp.cloud = 1;
                break;
            case 176: // patchy rain
            case 185: // patchy freezing drizzle
            case 263: // patchy light drizzle
            case 266: // light drizzle
            case 281: // freezing drizzle
            case 293: // patchy light rain
            case 296: // light rain
            case 299: // moderate rain at times
            case 311: // light freezing rain
                wp.sun = 1;
                wp.cloud = 1;
                wp.rain = 1;
                break;
            case 284: // heavy freezing drizzle
            case 302: // moderate rain
            case 305: // heavy rain at times
            case 308: // heavy rain
                wp.cloud = 1;
                wp.rain = 1;
                break;
            case 179: // patchy snow
            case 182: // patchy sleet
                wp.cloud = 1;
                wp.snow = 1;
                break;
            case 200: // thundery outbreaks
                wp.sun = 1;
                wp.cloud = 1;
                wp.thunder = 1;
                break;
            case 227: // blowing snow
            case 230: // blizzard
                wp.cloud = 1;
                wp.snow = 1;
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
