#include <stdio.h>
#include <unistd.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

static const char* TAG = "LED_BUTTON";

void app_main() {
   //Configurando saída (LED da placa, GPIO_NUM_2)
   gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DEF_OUTPUT);
   //Configurando entrada GPIO_NUM_4
   gpio_set_direction(GPIO_NUM_4, GPIO_MODE_DEF_INPUT);
   ESP_LOGI(TAG, "Exemplo - LED_BUTTON");
   while(true){
      //Lendo estado da entrada digital 
      int level = gpio_get_level(GPIO_NUM_4);
      ESP_LOGI(TAG, "Level: %d", level);
      //Alterando saída pra o mesmo estado da entrada
      gpio_set_level(GPIO_NUM_2, level); 
      sleep(1);
   }
}