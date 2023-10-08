#include "tasks/ir.h"
#include "tasks/render.h"
#include <esp_log.h>
#include <driver/rmt_rx.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

#define TAG "ir_task"

uint8_t nec_code_address;
uint8_t nec_code_command;

inline bool nec_check_in_range(uint32_t signal_duration, uint32_t spec_duration) {
    return (signal_duration < (spec_duration + IR_NEC_DECODE_MARGIN)) &&
           (signal_duration > (spec_duration - IR_NEC_DECODE_MARGIN));
}

bool nec_parse_logic0(rmt_symbol_word_t *rmt_nec_symbols) {
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ZERO_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ZERO_DURATION_1);
}

bool nec_parse_logic1(rmt_symbol_word_t *rmt_nec_symbols) {
    return nec_check_in_range(rmt_nec_symbols->duration0, NEC_PAYLOAD_ONE_DURATION_0) &&
           nec_check_in_range(rmt_nec_symbols->duration1, NEC_PAYLOAD_ONE_DURATION_1);
}

bool nec_parse_frame(rmt_symbol_word_t *rmt_nec_symbols) {
    rmt_symbol_word_t *cur = rmt_nec_symbols;
    uint16_t address = 0;
    uint16_t command = 0;
    bool valid_leading_code = nec_check_in_range(cur->duration0, NEC_LEADING_CODE_DURATION_0) &&
                              nec_check_in_range(cur->duration1, NEC_LEADING_CODE_DURATION_1);
    if (!valid_leading_code) {
        return false;
    }
    cur++;
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            address |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            address &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }
    for (int i = 0; i < 16; i++) {
        if (nec_parse_logic1(cur)) {
            command |= 1 << i;
        } else if (nec_parse_logic0(cur)) {
            command &= ~(1 << i);
        } else {
            return false;
        }
        cur++;
    }

    // validate addr and command
    if(((address >> 8) != (~address & 0xff)) ||
       ((command >> 8) != (~command & 0xff)))
       return false;

    // save address and command
    nec_code_address = address >> 8;
    nec_code_command = command >> 8;

    return true;
}

bool rmt_rx_done_callback(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data) {
    BaseType_t high_task_wakeup = pdTRUE;
    QueueHandle_t receive_queue = (QueueHandle_t)user_data;
    xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
    return high_task_wakeup == pdTRUE;
}

void ir_task(void* ignored) {
    QueueHandle_t receive_queue = xQueueCreate(1, sizeof(rmt_rx_done_event_data_t));

    // driver configs
    rmt_rx_channel_config_t rx_channel_cfg = {
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = IR_RESOLUTION_HZ,
        .mem_block_symbols = 64,
        .gpio_num = IR_RX_GPIO_NUM,
    };
    rmt_rx_event_callbacks_t cbs = {
        .on_recv_done = rmt_rx_done_callback,
    };
    rmt_receive_config_t receive_config = {
        .signal_range_min_ns = 1250,
        .signal_range_max_ns = 12000000,
    };

    // install drivers
    rmt_channel_handle_t rx_channel = NULL;
    ESP_ERROR_CHECK(rmt_new_rx_channel(&rx_channel_cfg, &rx_channel));
    ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(rx_channel, &cbs, receive_queue));
    ESP_ERROR_CHECK(rmt_enable(rx_channel));

    while(1) {
        // tell the driver to receive a frame
        rmt_symbol_word_t raw_symbols[64];
        ESP_ERROR_CHECK(rmt_receive(rx_channel, raw_symbols, sizeof(raw_symbols), &receive_config));

        // fetch frame from the queue
        rmt_rx_done_event_data_t rx_data;
        xQueueReceive(receive_queue, &rx_data, portMAX_DELAY);

        // decode NEC frame
        if(rx_data.num_symbols != 34 || !nec_parse_frame(rx_data.received_symbols))
            continue;
        ESP_LOGI(TAG, "nec addr=%2x cmd=%2x", nec_code_address, nec_code_command);

        // convert command to button name
        ir_button_t button = -1;
        switch(nec_code_command) {
            case 0xba: button = ir_exit; break;
            case 0xb8: button = ir_menu; break;
            case 0xf8: button = ir_door; break;
            case 0xbf: button = ir_ok; break;
            case 0xb9: button = ir_up; break;
            case 0xea: button = ir_down; break;
            case 0xbb: button = ir_left; break;
            case 0xbc: button = ir_right; break;
            case 0xe9: button = ir_0; break;
            case 0xf3: button = ir_1; break;
            case 0xe7: button = ir_2; break;
            case 0xa1: button = ir_3; break;
            case 0xf7: button = ir_4; break;
            case 0xe3: button = ir_5; break;
            case 0xa5: button = ir_6; break;
            case 0xbd: button = ir_7; break;
            case 0xad: button = ir_8; break;
            case 0xb5: button = ir_9; break;
            default:
                continue;
        }

        // send notification to renderer
        render_task_notification_t notif = {
            .type = rt_notif_ir,
            .u.ir_button = button,
        };
        xQueueSend(render_task_queue, &notif, 0);
    }
}
