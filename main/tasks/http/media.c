#include "tasks/http/media.h"
#include "tasks/http/co2.h"
#include "overall_config.h"
#include <time.h>
#include <sys/param.h>
#include <esp_log.h>
#include <string.h>
#include <time.h>

#define MEDIA_BUF_SIZE 128

render_task_notification_t _media_response_parse(cJSON* root) {
    render_task_notification_t notif;
    char* media_track = calloc(MEDIA_BUF_SIZE + 1, 1);
    char* media_album = calloc(MEDIA_BUF_SIZE + 1, 1);
    char* media_artist = calloc(MEDIA_BUF_SIZE + 1, 1);
    notif.type = rt_notif_media_info;
    notif.u.media_info.artist = media_artist;
    notif.u.media_info.album = media_album;
    notif.u.media_info.track = media_track;

    if(strcmp(cJSON_GetStringValue(cJSON_GetObjectItem(root, "state")), "playing") != 0) {
        notif.u.media_info.playing = false;
        return notif;
    }

    cJSON* attrs = cJSON_GetObjectItem(root, "attributes");
    if(cJSON_HasObjectItem(attrs, "media_title"))
        strncpy(media_track, cJSON_GetStringValue(cJSON_GetObjectItem(attrs, "media_title")), MEDIA_BUF_SIZE);
    if(cJSON_HasObjectItem(attrs, "media_album_name"))
        strncpy(media_album, cJSON_GetStringValue(cJSON_GetObjectItem(attrs, "media_album_name")), MEDIA_BUF_SIZE);
    if(cJSON_HasObjectItem(attrs, "media_artist"))
        strncpy(media_artist, cJSON_GetStringValue(cJSON_GetObjectItem(attrs, "media_artist")), MEDIA_BUF_SIZE);
    time_t media_dur = atoi(cJSON_GetStringValue(cJSON_GetObjectItem(attrs, "media_duration"))) * 1000;
    time_t media_pos = cJSON_GetNumberValue(cJSON_GetObjectItem(attrs, "media_position"));

    // parse ISO timestamp
    struct tm tm;
    int microsec; // ignored
    sscanf(cJSON_GetStringValue(cJSON_GetObjectItem(attrs, "media_position_updated_at")),
        "%d-%d-%dT%d:%d:%d.%d+00:00",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &microsec);
    tm.tm_year -= 1900;
    tm.tm_mon--;

    // convert time structure to epoch
    time_t update = timegm(&tm);
    time_t now;
    time(&now);
    media_dur = MAX(media_dur, 1);
    media_pos = MAX(MIN((media_pos + now - update) * 1000ll, media_dur), 0);
    ESP_LOGI("media", "upd=%lld now=%lld pos=%lld len=%lld", update, now, media_pos, media_dur);

    notif.u.media_info.pos = media_pos;
    notif.u.media_info.length = media_dur;
    notif.u.media_info.playing = true;

    return notif;
}
