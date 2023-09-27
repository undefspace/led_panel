#include "tasks/http/co2.h"

render_task_notification_t _co2_response_parse(cJSON* root) {
    int ppm = atoi(cJSON_GetStringValue(cJSON_GetObjectItem(root, "state")));
    return (render_task_notification_t){
        .type = rt_notif_co2_ppm,
        .u.co2_ppm = ppm,
    };
}
