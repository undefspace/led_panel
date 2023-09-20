#pragma once

#include <stdint.h>
#include "../olive.h"

static const uint8_t big_clock_image[] = {
    0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x95, 0x95, 0x95, 0x95, 0x00, 0x00, 0x00, 0x00, 
    0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0xff, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x95, 0x00, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x95, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0xff, 0xff, 0x95, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x95, 0x95, 0x00, 0x00, 0x00, 0xfd, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x09, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x95, 0x95, 0xfd, 0xfd, 0x95, 0x95, 0x00, 0x00, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x00, 0x00, 0xfd, 0xfd, 0x95, 0x00, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0x95, 0x00, 0xfd, 0xfd, 0x09, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0x95, 0x95, 0x95, 0x00, 0x00, 0x00, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 
    0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x00, 0x00, 
    0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x09, 0xff, 0xff, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0x95, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x64, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x02, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x95, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0xfd, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x02, 0x95, 0xfd, 0xfd, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x95, 0xfd, 0xfd, 0x95, 0x00, 0x00, 0x00, 0x00, 
};

static const size_t big_clock_glyph_map[] = {
    ['0'] = 0,
    ['1'] = 11,
    ['2'] = 22,
    ['3'] = 33,
    ['4'] = 44,
    ['5'] = 55,
    ['6'] = 66,
    ['7'] = 77,
    ['8'] = 88,
    ['9'] = 99,
    ['+'] = 110,
    ['-'] = 121,
    ['C'] = 132,
    ['W'] = 143,
};

static const Olivec_Font big_clock = {
    .alpha_image = big_clock_image,
    .glyph_map = big_clock_glyph_map,
    .width = 10,
    .height = 14,
    .stride = 11,
    .img_stride = 154
};