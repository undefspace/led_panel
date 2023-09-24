#include "elements/media.h"
#include "fonts/small_clock.h"
#include <stdio.h>

media_state_t media_state;

void media_draw(Olivec_Canvas canvas) {
    if(!media_state.playing)
        return;

    // darken spectrum
    olivec_rect(canvas, 0, 0, media.width, media.height, MEDIA_BG);

    // draw progress bar
    int progress_width = media.width * media_state.pos / media_state.length;
    olivec_rect(canvas, 0, 0, media.width, 2, MEDIA_PROGRESS_BG);
    olivec_rect(canvas, 0, 0, progress_width, 2, MEDIA_PROGRESS_FG);

    // draw times
    int pos_min = media_state.pos / 60000, pos_sec = (media_state.pos / 1000) % 60;
    int len_min = media_state.length / 60000, len_sec = (media_state.length / 1000) % 60;
    char* pos[16], len[16];
    sprintf(pos, "%02d:%02d", pos_min, pos_sec);
    sprintf(len, "%02d:%02d", len_min, len_sec);
    olivec_text(canvas, pos, 0, 3, small_clock, MEDIA_PROGRESS_FG);
    olivec_text(canvas, len, 45, 3, small_clock, MEDIA_PROGRESS_FG);
}
