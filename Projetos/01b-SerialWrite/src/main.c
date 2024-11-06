#include <stdio.h>
#include <string.h>
#include "driver/uart.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "esp_log.h"

/**
 * 00-Serial Echo, adaptado do código fonte do fabricante.
 */

#define ECHO_TEST_TXD (GPIO_NUM_1)
#define ECHO_TEST_RXD (GPIO_NUM_3)
#define ECHO_TEST_RTS (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS (UART_PIN_NO_CHANGE)

#define ECHO_UART_PORT_NUM      (UART_NUM_0)
#define ECHO_UART_BAUD_RATE     (9600)
#define ECHO_TASK_STACK_SIZE    (configMINIMAL_STACK_SIZE)

static const char *TAG = "UART_ECHO";

#define BUF_SIZE (256)

void app_main(void)
{
    /* Estrutura de dados que Configura a porta serial*/
    uart_config_t uart_config = {
        .baud_rate = ECHO_UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    int intr_alloc_flags = 0;

    //Inicializa o driver da porta serial
    ESP_ERROR_CHECK(uart_driver_install(ECHO_UART_PORT_NUM, BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));
    //Configura a porta com os dados da estrutura
    ESP_ERROR_CHECK(uart_param_config(ECHO_UART_PORT_NUM, &uart_config));
    //Indica os pinos para uso na comunicação
    ESP_ERROR_CHECK(uart_set_pin(ECHO_UART_PORT_NUM, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    //Buffer de dados
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    //Loop para leitura e escrita
    while (1) {
        //Criando buffer para envio
        sprintf(data, "%s", "Testando 1, 2, 3");
        //Escrevendo dados na porta serial
        uart_write_bytes(ECHO_UART_PORT_NUM, (const char *) data, strlen(data));
        //Zerando buffer
        memset(data, 0x00, BUF_SIZE);
        //Lendo dados
        int len = uart_read_bytes(ECHO_UART_PORT_NUM, data, (BUF_SIZE - 1), 5000 / portTICK_PERIOD_MS);
        if (len) {
            // Se houver dados, mostra o que foi recebido
            data[len] = '\n';
            ESP_LOGI(TAG, "Recv str: %s", (char *) data);
        }
    }
}