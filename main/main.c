#include <time.h>
#include <sys/time.h>
#include "overall_config.h"
#include "wifi_creds.h"
#include "render_task.h"
#include "weather_fetch_task.h"
#include <esp_wifi.h>
#include <esp_log.h>
#include <leddisplay.h>
#include <nvs_flash.h>
#include <stdlib.h>
#include <esp_sntp.h>
#include <sntp.h>

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if(event_base == WIFI_EVENT)
        if(event_id == WIFI_EVENT_STA_START || event_id == WIFI_EVENT_STA_DISCONNECTED)
            esp_wifi_connect();

    if((event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) ||
       (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)) {
        render_task_notification_t notif = {
            .type = rt_notif_wifi_status,
            .u.wifi_connected = event_base == IP_EVENT
        };
        xQueueSend(render_task_queue, &notif, 0);
    }
}

void app_main(void) {
    // initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // connect to to Wi-Fi
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t default_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&default_cfg));
    esp_event_handler_instance_t event_handler_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
        ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &event_handler_instance));
    esp_event_handler_instance_t ip_event_handler_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
        ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &event_handler_instance));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // set up NTP
    setenv("TZ", TZ, 1);
    tzset();
    sntp_setservername(0, NTP_SERVER);
    sntp_setoperatingmode(SNTP_OPMODE_POLL);
    sntp_init();

    // set up panel
    ESP_ERROR_CHECK(leddisplay_init());

    // create tasks
    xTaskCreate(render_task, "renderer", 8192, NULL, 10, NULL);
    xTaskCreate(weather_fetch_task, "weather", 8192, NULL, 10, NULL);

    // set up strip
    // FastLED.addLeds<WS2812B, STRIP_PIN_D>(strip, STRIP_WIDTH);
}
