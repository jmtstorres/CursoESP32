#include "wrp_wifi.h"
#include "wrp_http_client.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

//Alterar para o da rede a conectar
#define EXAMPLE_ESP_WIFI_SSID      "SSID"
#define EXAMPLE_ESP_WIFI_PASS      "password"

/*
    Para uso de certificados próprios, os mesmos devem ser declarados nos arquivos de configuraçao
    src/CMakeLists.txt e platformio.ini
*/

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
    while(wifi_status() != WRP_WIFI_ST_GOT_IP){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "Conectado.");

    ESP_LOGI(TAG, "Fazendo request");
    rc = wrp_http_get("http://httpbin.org/get");
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }

    rc = wrp_http_post("Teste", "http://httpbin.org/post");
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
}