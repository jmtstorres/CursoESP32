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

//Compilado como C++ para compatibilidade da biblioteca
//do display
extern "C" void app_main();

void app_main() {

	//Handle do display
    SSD1306_t dev;

	//Mostra configuracoes do I2C
	ESP_LOGI(TAG_MAIN, "INTERFACE is i2c");
	ESP_LOGI(TAG_MAIN, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(TAG_MAIN, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(TAG_MAIN, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);

	//Inicializa o I2C
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);

	//Inicia comunicacao com o display
	ssd1306_init(&dev, 128, 64);
	//Limpa todos os pixels da tela
	ssd1306_clear_screen(&dev, false);
	//Escreve a String na primeira linha do display
    ssd1306_display_text(&dev, 0, "Hello World", 12, false);
}