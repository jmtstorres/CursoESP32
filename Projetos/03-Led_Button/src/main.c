#include <stdio.h>
#include <unistd.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>

static const char* TAG = "LED_BUTTON";

void app_main() {  
   gpio_set_direction(GPIO_NUM_2, GPIO_MODE_DEF_OUTPUT);
   gpio_set_direction(GPIO_NUM_4, GPIO_MODE_DEF_INPUT);
   ESP_LOGI(TAG, "Exemplo - LED_BUTTON");
   while(true){
      //Alterando estado para alto (HIGH) 
      int level = gpio_get_level(GPIO_NUM_4);
      //Alterando estado para alto (HIGH) 
      ESP_LOGI(TAG, "Level: %d", level);
      gpio_set_level(GPIO_NUM_2, level); 
      sleep(1);
   }
}