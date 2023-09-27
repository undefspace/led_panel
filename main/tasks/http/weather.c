#include "tasks/http/weather.h"

render_task_notification_t _weather_response_parse(cJSON* root) {
    int icon_id;
    char time_of_day;
    cJSON* weather_list = cJSON_GetObjectItem(root, "weather");
    cJSON* weather = cJSON_GetArrayItem(weather_list, 0);
    cJSON* main = cJSON_GetObjectItem(root, "main");
    float temp = cJSON_GetNumberValue(cJSON_GetObjectItem(main, WEATHER_USE_FEELS_LIKE ? "feels_like" : "temp"));
    sscanf(cJSON_GetStringValue(cJSON_GetObjectItem(weather, "icon")), "%02d%c", &icon_id, &time_of_day);

    // transform icon codes
    render_task_notification_t notification = {
        .type = rt_notif_weather,
        .u.weather.temperature = temp,
    };
    #define wp notification.u.weather
    switch(icon_id) {
        case 1:
            wp.sun = 1;
            break;
        case 2:
            wp.sun = 1;
            wp.cloud = 1;
            break;
        case 3:
        case 4:
            wp.cloud = 1;
            break;
        case 9:
            wp.cloud = 1;
            wp.rain = 1;
            break;
        case 10:
            wp.sun = 1;
            wp.cloud = 1;
            wp.rain = 1;
            break;
        case 11:
            wp.cloud = 1;
            wp.thunder = 1;
            break;
        case 13:
            wp.snow = 1;
            break;
        case 50:
            wp.mist = 1;
            break;
    }
    #undef wp

    return notification;
}
