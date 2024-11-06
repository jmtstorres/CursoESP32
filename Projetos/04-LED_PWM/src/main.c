#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include <math.h>
#include "esp_log.h"

#define SAMPLE_CNT 32
//Declarando qual canal ADC será usado
static const adc1_channel_t adc_channel = ADC_CHANNEL_4;
#define LEDC_GPIO GPIO_NUM_2
static ledc_channel_config_t ledc_channel;

static void init_hw(void)
{
    //Configurando largura de 10 bits
    adc1_config_width(ADC_WIDTH_BIT_10);
    //Configurando atenuacao
    adc1_config_channel_atten(adc_channel, ADC_ATTEN_DB_11);
    //Preenchendo configuracao do timer
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    //Configurando timer
    ledc_timer_config(&ledc_timer);

    //Preenchendo configuracao do PWM
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = LEDC_GPIO;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.hpoint = 0;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    //Configurando PWM
    ledc_channel_config(&ledc_channel);
}

void app_main()
{
    //Inicializa o ADC/PWM
    init_hw();
    //Marca o tempo para geracao da senoide
    double time = 0.0;
    while (1)
    {  
        time += 0.1;
        //calcula o valor da saida
        double adc_val = (sin(time) + 1.0)*SAMPLE_CNT/2.0;
        ESP_LOGI("TAG", "%f\n", adc_val);
        //Altera o valor em memória para depois atualizar a saída do PWM
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, adc_val);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        //Delay
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}