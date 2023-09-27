#include "tasks/http/temp.h"

render_task_notification_t _temp_response_parse(cJSON* root) {
    float temp = atof(cJSON_GetStringValue(cJSON_GetObjectItem(root, "state")));
    return (render_task_notification_t){
        .type = rt_notif_temp,
        .u.indoor_temp = temp,
    };
}
