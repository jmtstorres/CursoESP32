#include <stdio.h>
#include <unistd.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

static const char* TAG = "LED_BLINK";

void app_main(void) // Main
{
    //Configura GPIO como saída (LED da placa)
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DEF_OUTPUT);
    while(true){
        ESP_LOGI(TAG, "Exemplo - LED on");
        //Alterando estado para alto (HIGH)  
        gpio_set_level(GPIO_NUM_2, 1);  
        //Aguardando 1 segundo  
        sleep(1);  
        ESP_LOGI(TAG, "Exemplo - LED off");
        //Alterando estado para alto (HIGH)  
        gpio_set_level(GPIO_NUM_2, 0); 
        //Aguardando 1 segundo  
        sleep(1);
    }
}