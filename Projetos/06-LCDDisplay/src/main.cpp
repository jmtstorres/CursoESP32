#define CONFIG_LEGACY_DRIVER 1
#define LEGACY_DRIVER 1

#include <stdio.h>
#include <string.h>
#include "esp_err.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "ssd1306.h"

//https://github.com/nopnop2002/esp-idf-ssd1306
//habilitar driver legacy i2c:
//abrir novo terminal pio e rodar "pio run -t menuconfig"
//opcao: SSD1306

#define TAG_MAIN "ESP_TEST"

extern "C" void app_main();

void app_main() {

    SSD1306_t dev;

	ESP_LOGI(TAG_MAIN, "INTERFACE is i2c");
	ESP_LOGI(TAG_MAIN, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG_MAIN, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(TAG_MAIN, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);

	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);

	ssd1306_init(&dev, 128, 64);
	ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, "Hello World", 12, false);
}