#include "tasks/media_server.h"
#include "tasks/render.h"
#include <esp_http_server.h>
#include <freertos/FreeRTOS.h>
#include <string.h>
#include <lwip/sockets.h>
#include <esp_log.h>

esp_err_t _media_server_endpoint(httpd_req_t* req) {
    // read content
    char content[512];
    int ret = httpd_req_recv(req, content, req->content_len);
    if(ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
            httpd_resp_send_408(req);
        return ESP_FAIL;
    }
    content[req->content_len] = 0;

    // parse content
    int i = 0;
    char* strings[6];
    char* token = strtok(content, "\n");
    while(token) {
        strings[i] = malloc(strlen(token) + 1);
        strcpy(strings[i++], token);
        token = strtok(NULL, "\n");
    }
    uint32_t length = atoi(strings[3]);
    bool playing = atoi(strings[4]);
    uint32_t pos = atoi(strings[5]);
    free(strings[3]);
    free(strings[4]);
    free(strings[5]);

    // send notification
    render_task_notification_t notif = {
        .type = rt_notif_media_info,
        .u.media_info = {
            .track = strings[0],
            .album = strings[1],
            .artist = strings[2],
            .length = length,
            .playing = playing,
            .pos = pos,
        }
    };
    xQueueSend(render_task_queue, &notif, 0);

    // send response
    const char* resp = "OK";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void media_server_task(void* ignored) {
    // create server
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    httpd_uri_t endpoint = {
        .uri = "/media",
        .method = HTTP_POST,
        .handler = _media_server_endpoint,
        .user_ctx = NULL
    };
    ESP_ERROR_CHECK(httpd_start(&server, &config));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &endpoint));

    while(1) {
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}
