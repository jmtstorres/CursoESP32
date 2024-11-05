#include <wrp_http_client.h>

esp_err_t _http_event_handler(esp_http_client_event_t *evt);

char local_response_buffer[1024] = {0};
esp_http_client_handle_t client = -1;
char _host[1024] = {0};
int _port = -1;

//https://stackoverflow.com/questions/77510803/how-to-make-a-simple-https-server-in-python-3x

esp_http_client_handle_t wrp_http_init_client(const char *host, int port){
    ESP_LOGI(TAG, "Host: %s", host);
    sprintf(_host, "%s", host);
    _port = port;
    esp_http_client_config_t config = {
        .host = _host,
        .path = "/",
        .port = port,
        .user_data = local_response_buffer,
        .auth_type = HTTP_AUTH_TYPE_NONE,
        .event_handler = _http_event_handler,
        //.skip_cert_common_name_check = true,
        .client_cert_pem = (const char *)client_certificate_start,
    };
    client = esp_http_client_init(&config);
    if (client < 0){
        return client;
    }
    return ESP_OK;
}

int wrp_http_post(const char *post_data, const char *url){
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = _http_event_handler,
        .user_data = local_response_buffer,
        .auth_type = HTTP_AUTH_TYPE_NONE,
        //.crt_bundle_attach = esp_crt_bundle_attach,
        .cert_pem = (const char *)client_certificate_start,
        //.client_key_pem = (const char *)client_key_start,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_post_field(client, post_data, strlen(post_data));
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTPS Status = %d, content_length = %"PRId64,
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
    } else {
        ESP_LOGE(TAG, "Error perform http request %s", esp_err_to_name(err));
    }
    esp_http_client_close(client);
    return esp_http_client_cleanup(client);
}

int wrp_http_get(const char *url){
    esp_http_client_set_url(client, url);
    esp_http_client_set_method(client, HTTP_METHOD_GET);
    //esp_http_client_set_header(client, "Content-Type", "text/plain");
    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "HTTP GET Status = %d, content_length = %llu",
                esp_http_client_get_status_code(client),
                esp_http_client_get_content_length(client));
        esp_http_client_read_response(client, local_response_buffer, 1024);
        ESP_LOGI(TAG, "HTTP GET response: %s", local_response_buffer);
        memset(local_response_buffer, 0x00, sizeof(local_response_buffer));
    } else {
        ESP_LOGE(TAG, "HTTP GET request failed: %s", esp_err_to_name(err));
        wrp_http_close();
    }
    return err;
}

int wrp_http_close(){
    esp_http_client_cleanup(client);
    return esp_http_client_close(client);
}

esp_err_t _http_event_handler(esp_http_client_event_t *evt)
{
    static char *output_buffer;
    static int output_len;
    switch(evt->event_id) {
        case HTTP_EVENT_ERROR:
            ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
            break;
        case HTTP_EVENT_HEADER_SENT:
            ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
            break;
        case HTTP_EVENT_ON_HEADER:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
            if (!esp_http_client_is_chunked_response(evt->client)) {
                if (evt->user_data) {
                    memcpy(evt->user_data + output_len, evt->data, evt->data_len);
                } else {
                    if (output_buffer == NULL) {
                        output_buffer = (char *) malloc(esp_http_client_get_content_length(evt->client));
                        output_len = 0;
                        if (output_buffer == NULL) {
                            ESP_LOGI(TAG, "Failed to allocate memory for output buffer");
                            return ESP_FAIL;
                        }
                    }
                    memcpy(output_buffer + output_len, evt->data, evt->data_len);
                }
                output_len += evt->data_len;
            }
            break;
        case HTTP_EVENT_ON_FINISH:
            ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
            if (output_buffer != NULL) {
                ESP_LOGI(TAG, "%s", output_buffer);
                free(output_buffer);
                output_buffer = NULL;
            }
            output_len = 0;
            break;
        case HTTP_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            wrp_http_init_client(_host, _port);
            break;
        case HTTP_EVENT_REDIRECT:
            ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
            wrp_http_init_client(_host, _port);
            break;
    }
    return ESP_OK;
}