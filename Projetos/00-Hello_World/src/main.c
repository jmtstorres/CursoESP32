#include <stdio.h>
#include <string.h>
#include "esp_log.h"

//Tag para mostra em log
#define TAG "HELLO"

void app_main() {
    //Escrevendo Hello World na porta Serial
    ESP_LOGI(TAG, "Hello World!");
}