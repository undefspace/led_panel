#pragma once

#include "overall_config.h"
#include <stdint.h>
#include <stddef.h>

#define IR_RESOLUTION_HZ            1000000
#define IR_NEC_DECODE_MARGIN        200
#define NEC_LEADING_CODE_DURATION_0 9000
#define NEC_LEADING_CODE_DURATION_1 4500
#define NEC_PAYLOAD_ZERO_DURATION_0 560
#define NEC_PAYLOAD_ZERO_DURATION_1 560
#define NEC_PAYLOAD_ONE_DURATION_0  560
#define NEC_PAYLOAD_ONE_DURATION_1  1690

typedef enum {
    ir_0 = 0, ir_1, ir_2, ir_3, ir_4, ir_5, ir_6, ir_7, ir_8, ir_9,
    ir_up, ir_down, ir_left, ir_right,
    ir_exit, ir_menu, ir_ok,
    ir_door,
} ir_button_t;

static inline const char* ir_button_t_to_str(ir_button_t btn) {
    static const char* strings[] = {
        "ir_0", "ir_1", "ir_2", "ir_3", "ir_4", "ir_5", "ir_6", "ir_7", "ir_8", "ir_9",
        "ir_up", "ir_down", "ir_left", "ir_right",
        "ir_exit", "ir_menu", "ir_ok",
        "ir_door",
    };
    return strings[btn];
}

void ir_task(void* ignored);
