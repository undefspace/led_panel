#include "tasks/http/co2.h"
#include "overall_config.h"
#include <time.h>
#include <sys/param.h>
#include <esp_log.h>
#include <string.h>

uint16_t ppm_buffer[PANEL_WIDTH] = {0};

time_t timegm(struct tm* tm) {
    time_t tStampBadLocaltime = mktime(tm);
    struct tm tmUTC;
    struct tm tmLocaltime;
    gmtime_r(&tStampBadLocaltime, &tmUTC);
    localtime_r(&tStampBadLocaltime, &tmLocaltime);
    time_t tstampBadUTC = mktime(&tmUTC);
    time_t tstampLocaltime = mktime(&tmLocaltime);
    time_t tLocalOffset = tstampLocaltime - tstampBadUTC;
    return tStampBadLocaltime + tLocalOffset;
}

void _co2_url_prepare(char* buf, const char* template) {
    // get time
    time_t now;
    time(&now);

    // subtract PANEL_WIDTH minutes
    now -= PANEL_WIDTH * 60;
    struct tm time;
    localtime_r(&now, &time);

    // format time and insert into request
    char formatted_time[64];
    strftime(formatted_time, 64, "%FT%T%z", &time);
    sprintf(buf, template, formatted_time);
}

render_task_notification_t _co2_response_parse(cJSON* root) {
    cJSON* entries = cJSON_GetArrayItem(root, 0);
    if(!entries)
        return (render_task_notification_t){0};

    // parse json
    memset(ppm_buffer, 0, sizeof(ppm_buffer));
    time_t last_ts = -1;
    for(int i = cJSON_GetArraySize(entries) - 1; i > 0; i--) {
        cJSON* entry = cJSON_GetArrayItem(entries, i);

        // parse ISO timestamp
        struct tm time;
        int microsec; // ignored
        sscanf(cJSON_GetStringValue(cJSON_GetObjectItem(entry, "last_changed")),
            "%d-%d-%dT%d:%d:%d.%d+00:00",
            &time.tm_year, &time.tm_mon, &time.tm_mday,
            &time.tm_hour, &time.tm_min, &time.tm_sec, &microsec);
        time.tm_year -= 1900;
        time.tm_mon--;

        // convert time structure to epoch
        time_t ts = timegm(&time);
        if(last_ts == -1)
            last_ts = ts;

        // assign to the appropriate cell
        int mins_back = MIN(MAX((last_ts - ts) / 60, 0), PANEL_WIDTH - 1);
        ppm_buffer[PANEL_WIDTH - 1 - mins_back] = atoi(cJSON_GetStringValue(cJSON_GetObjectItem(entry, "state")));
    }

    // fill in the gaps
    uint16_t ppm_last = ppm_buffer[0];
    for(int i = 1; i < PANEL_WIDTH; i++) {
        if(ppm_buffer[i])
            ppm_last = ppm_buffer[i];
        else
            ppm_buffer[i] = ppm_last;
    }

    // create notification
    return (render_task_notification_t){
        .type = rt_notif_co2_ppm,
        .u.ppm_buffer = ppm_buffer,
    };
}
