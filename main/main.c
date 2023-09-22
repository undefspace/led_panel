#include <time.h>
#include <sys/time.h>
#include "overall_config.h"
#include "wifi_creds.h"
#include "render_task.h"
#include "weather_fetch_task.h"
#include "fft_task.h"
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

#define min(a, b) (a <= b ? a : b)
#define max(a, b) (a >= b ? a : b)

// declarations
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void app_main(void);
void brightness_task(void* ignored);

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
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
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("pool.ntp.org");
    esp_netif_sntp_init(&config);

    // set up panel
    ESP_ERROR_CHECK(leddisplay_init());

    // create tasks
    xTaskCreate(render_task,        "renderer", 4096, NULL, 10, NULL);
    xTaskCreate(weather_fetch_task, "weather", 8192, NULL, 10, NULL);
    xTaskCreate(fft_task,           "fft", 4096, NULL, 10, NULL);
    // xTaskCreate(brightness_task,    "brightness", 2048, NULL, 10, NULL);

    // set up strip
    // FastLED.addLeds<WS2812B, STRIP_PIN_D>(strip, STRIP_WIDTH);
}

void brightness_task(void* ignored) {
    int latched_brightness = 100;

    // set up ADC
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH_12,
        .atten = ADC_ATTEN_DB_0,
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_5, &config));

    // set up ADC calibration
    adc_cali_handle_t adc_cali_handle = NULL;
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .atten = config.atten,
        .bitwidth = config.bitwidth,
    };
    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle));

    while(1) {
        // read ADC
        int adc_raw_total, adc_millivolts;
        for(int i = 0; i < 10; i++) {
            int adc_raw;
            ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_5, &adc_raw));
            adc_raw_total += adc_raw;
        }
        adc_raw_total /= 10;
        ESP_ERROR_CHECK(adc_cali_raw_to_voltage(adc_cali_handle, adc_raw_total, &adc_millivolts));

        // calculate brightness
        int lux = adc_millivolts / 5;
        int brightness = min(max(lux * 4, 10), 255);

        // latch brightness
        if(abs(brightness - latched_brightness) > 30) {
            latched_brightness = brightness;
            // send notification to renderer
            render_task_notification_t notif = {
                .type = rt_notif_brightness,
                .u.brightness = brightness
            };
            xQueueSend(render_task_queue, &notif, 0);
        }

        // repeat after 100ms
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
