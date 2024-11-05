#include "wrp_wifi.h"
#include "wrp_http_srv.h"

#define EXAMPLE_ESP_WIFI_SSID      "CLARO_2G5106B1"
#define EXAMPLE_ESP_WIFI_PASS      "hcT1uuN2vf"

void app_main() {
    //Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Iniciando...");
    int rc = wifi_init();
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }

    rc = wifi_connect(EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    ESP_LOGI(TAG, "Conectando...");
    while(wifi_status() != WRP_WIFI_ST_CONNECTED){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "Conectado.");

    httpd_handle_t handle = start_webserver();
}