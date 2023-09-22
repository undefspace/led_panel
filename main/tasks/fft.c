#include "tasks/fft.h"
#include <fft.h>
#include <esp_adc/adc_continuous.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <math.h>
#include "tasks/render.h"

#define TAG "fft_task"

static TaskHandle_t fft_task_handle;
float fft_output[FFT_SIZE * 2];

static bool IRAM_ATTR _adc_conversion_done(adc_continuous_handle_t handle, const adc_continuous_evt_data_t *edata, void *user_data) {
    BaseType_t mustYield = pdFALSE;
    vTaskNotifyGiveFromISR(fft_task_handle, &mustYield);
    return (mustYield == pdTRUE);
}

void fft_task(void* ignored) {
    // set task handle
    fft_task_handle = xTaskGetCurrentTaskHandle();

    // initialize FFT
    fft_config_t* fft_config = fft_init(FFT_SIZE, FFT_REAL, FFT_FORWARD, NULL, fft_output);

    // initialize ADC
    adc_continuous_handle_t handle = NULL;
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = FFT_SIZE * SOC_ADC_DIGI_RESULT_BYTES,
        .conv_frame_size = FFT_SIZE * SOC_ADC_DIGI_RESULT_BYTES,
    };
    adc_digi_pattern_config_t adc_pattern = {
        .atten = ADC_ATTEN_DB_0,
        .channel = ADC_CHANNEL_4,
        .unit = ADC_UNIT_1,
        .bit_width = ADC_BITWIDTH_12,
    };
    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = SPECTRUM_FPS * FFT_SIZE,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE1,
        .pattern_num = 1,
        .adc_pattern = &adc_pattern,
    };
    adc_continuous_evt_cbs_t adc_cbs = {
        .on_conv_done = _adc_conversion_done,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));
    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));
    ESP_ERROR_CHECK(adc_continuous_register_event_callbacks(handle, &adc_cbs, NULL));
    ESP_ERROR_CHECK(adc_continuous_start(handle));

    // conversion frame buffer
    uint8_t conv_frame_buf[FFT_SIZE * SOC_ADC_DIGI_RESULT_BYTES] = {0};
    uint32_t conv_frame_buf_size = 0;

    while(1) {
        // wait for conversion result
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // read available conversion frames
        while(1) {
            esp_err_t err = adc_continuous_read(handle, conv_frame_buf, FFT_SIZE * SOC_ADC_DIGI_RESULT_BYTES, &conv_frame_buf_size, 0);
            if(err != ESP_OK)
                break;

            // interpret conversion frame
            for(int i = 0; i < conv_frame_buf_size; i += SOC_ADC_DIGI_RESULT_BYTES) {
                adc_digi_output_data_t* ptr = (adc_digi_output_data_t*)&conv_frame_buf[i];
                if (ptr->type1.channel != ADC_CHANNEL_4) {
                    ESP_LOGW(TAG, "invalid data chan=%d sample=%d", ptr->type1.channel, ptr->type1.data);
                    continue;
                }

                // convert 12-bit int to float
                fft_config->input[i / SOC_ADC_DIGI_RESULT_BYTES] = (ptr->type1.data - 1550) / 4095.0f;
            }

            // do fft
            fft_execute(fft_config);
            
            // compute magnitudes
            for(int i = 0; i < FFT_SIZE; i++) {
                float a = fft_output[i * 2];
                float b = fft_output[(i * 2) + 1];
                float m = sqrt((a * a) + (b * b));
            }

            // send to renderer
            render_task_notification_t notif = {
                .type = rt_notif_fft,
                .u.fft_buffer = fft_output,
            };
            xQueueSend(render_task_queue, &notif, 0);
        }
    }
}
