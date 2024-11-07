#include "wrp_wifi.h"

//Alterar para o da rede a conectar
#define EXAMPLE_ESP_WIFI_SSID      "SSID"
#define EXAMPLE_ESP_WIFI_PASS      "password"

void app_main() {
    //Inicializando NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "Iniciando Wifi...");
    int rc = wifi_init();
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }

    //Conectando ao SSID definido
    rc = wifi_connect(EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    ESP_LOGI(TAG, "Conectando...");
    while(wifi_status() != WRP_WIFI_ST_CONNECTED){
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    ESP_LOGI(TAG, "Conectado.");
}