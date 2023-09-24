#pragma once

#include "ui_element.h"
#include "elements/spectrum.h"

#define MEDIA_BG          0x80000000
#define MEDIA_PROGRESS_FG 0xffffffff
#define MEDIA_PROGRESS_BG 0xff565656
#define MEDIA_TRACK       0xffffffff
#define MEDIA_ARTIST      0xff888888

typedef struct {
    char* track;
    char* album;
    char* artist;
    uint32_t length;
    uint32_t pos;
    bool playing;
} media_state_t;

extern media_state_t media_state;

void media_draw(Olivec_Canvas canvas);
static const ui_element_t media = {
    .x = 0, .y = 47,
    .width = 64, .height = SPECTRUM_HEIGHT,
    .draw = media_draw
};