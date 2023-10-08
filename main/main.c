#include <time.h>
#include <sys/time.h>
#include "overall_config.h"
#include "wifi_creds.h"
#include "tasks/render.h"
#include "tasks/http.h"
#include "tasks/fft.h"
#include "tasks/led.h"
#include "tasks/media_server.h"
#include "tasks/ir.h"
#include <esp_wifi.h>
#include <esp_log.h>
#include <leddisplay.h>
#include <nvs_flash.h>
#include <stdlib.h>
#include <esp_netif.h>
#include <esp_netif_sntp.h>
#include <esp_adc/adc_oneshot.h>
#include <esp_adc/adc_cali.h>
#include <esp_adc/adc_cali_scheme.h>
#include <stdlib.h>

// declarations
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void app_main(void);
void brightness_task(void* ignored);

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if(event_base == WIFI_EVENT && (event_id == WIFI_EVENT_STA_START || event_id == WIFI_EVENT_STA_DISCONNECTED))
        esp_wifi_connect();

    if((event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) ||
       (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)) {
        if(event_base == IP_EVENT) {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI("main", "got ip: "IPSTR, IP2STR(&event->ip_info.ip));
        }
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
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(NTP_SERVER);
    esp_netif_sntp_init(&config);

    // set up panel
    ESP_ERROR_CHECK(leddisplay_init());

    // create tasks
    xTaskCreate(render_task, "renderer", 4096, NULL, 10, NULL);
    xTaskCreate(http_task, "http", 10240, NULL, 10, NULL);
    xTaskCreate(fft_task, "fft", 4096, NULL, 10, NULL);
    xTaskCreate(media_server_task, "media_server", 2048, NULL, 10, NULL);
    xTaskCreate(ir_task, "ir", 4096, NULL, 10, NULL);
    // xTaskCreatePinnedToCore(led_task, "led", 2048, NULL, configMAX_PRIORITIES - 1, NULL, 1);
    // TODO: make the LED controller use the new RMT driver

    // print heap stats every minute
    while(1) {
        heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
}
