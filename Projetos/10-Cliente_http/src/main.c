#include "wrp_wifi.h"
#include "wrp_http_client.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

#define EXAMPLE_ESP_WIFI_SSID      "CLARO_2G5106B1"
#define EXAMPLE_ESP_WIFI_PASS      "hcT1uuN2vf"

static void example_https(void *pvParameter)
{
    while (1) {
        int level = gpio_get_level(GPIO_NUM_4);
        printf("level %d\n", level);
        if(level == 1){
            ESP_LOGI(TAG, "Fazendo request");
            int rc = wrp_http_post("Teste", "https://192.168.0.108:5000?param1=value1&param2=value2");
            if(rc != ESP_OK){
                ESP_LOGI(TAG, "Erro: %d", rc);
            }
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

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

    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_DEF_INPUT);
    xTaskCreate(&example_https, "example_https", 4096, NULL, 5, NULL);
}