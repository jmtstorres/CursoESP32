#include <stdio.h>
#include <unistd.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

static const char* TAG = "LED_BLINK";

void app_main(void) // Main
{
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DEF_OUTPUT);
    ESP_LOGI(TAG, "Exemplo - Hello World");
    while(true){
        //Alterando estado para alto (HIGH)  
        ESP_LOGI(TAG, "Exemplo - LED on");
        gpio_set_level(GPIO_NUM_2, 1);  
        //Aguardando 1 segundo  
        sleep(1);  
        //Alterando estado para alto (HIGH)  
        ESP_LOGI(TAG, "Exemplo - LED off");
        gpio_set_level(GPIO_NUM_2, 0); 
        //Aguardando 1 segundo  
        sleep(1);
    }
}