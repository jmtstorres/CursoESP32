#include "wrp_http_srv.h"
/* 
 * Adaptado de https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/esp_http_server.html
 */

esp_err_t get_handler(httpd_req_t *req);
esp_err_t post_handler(httpd_req_t *req);

httpd_uri_t uri_get = {
    .uri      = "/urig",
    .method   = HTTP_GET,
    .handler  = get_handler,
    .user_ctx = NULL
};

httpd_uri_t uri_post = {
    .uri      = "/urip",
    .method   = HTTP_POST,
    .handler  = post_handler,
    .user_ctx = NULL
};

//curl -X GET <ip>
esp_err_t get_handler(httpd_req_t *req)
{
    const char resp[] = "<!DOCTYPE html>\
                    <html lang=\"en\">\
                    <head>\
                    <title>ESP32 Example</title>\
                    <meta charset=\"UTF-8\">\
                    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
                    <style>\
                    body {\
                        font-family: Arial, Helvetica, sans-serif;\
                    }\
                    </style>\
                    </head>\
                    <body>\
                    <h1>ESP32 Example</h1>\
                    <p>Get Request.</p>\
                    </body>\
                    </html>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

//curl -X POST <ip> -d 'dados'
esp_err_t post_handler(httpd_req_t *req)
{
    char content[100];

    size_t recv_size = MIN(req->content_len, sizeof(content));

    int ret = httpd_req_recv(req, content, recv_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            httpd_resp_send_408(req);
        }
        return ESP_FAIL;
    }

    const char resp[] = "<!DOCTYPE html>\
                        <html lang=\"en\">\
                        <head>\
                        <title>ESP32 Example</title>\
                        <meta charset=\"UTF-8\">\
                        <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
                        <style>\
                        body {\
                            font-family: Arial, Helvetica, sans-serif;\
                        }\
                        </style>\
                        </head>\
                        <body>\
                        <h1>ESP32 Example</h1>\
                        <p>Post Request.</p>\
                        </body>\
                        </html>";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
    }
    return server;
}

void stop_webserver(httpd_handle_t server)
{
    if (server) {
        httpd_stop(server);
    }
}