#include <stdio.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "driver/ledc.h"
#include <math.h>
#include "esp_log.h"

#define SAMPLE_CNT 32
static const adc1_channel_t adc_channel = ADC_CHANNEL_4;
static const adc2_channel_t adc2_channel = ADC2_CHANNEL_2;
#define LEDC_GPIO GPIO_NUM_2
static ledc_channel_config_t ledc_channel;

static void init_hw1(void)
{
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(adc_channel, ADC_ATTEN_DB_11);
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_10_BIT,
        .freq_hz = 1000,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);
    ledc_channel.channel = LEDC_CHANNEL_0;
    ledc_channel.duty = 0;
    ledc_channel.gpio_num = LEDC_GPIO;
    ledc_channel.speed_mode = LEDC_HIGH_SPEED_MODE;
    ledc_channel.hpoint = 0;
    ledc_channel.timer_sel = LEDC_TIMER_0;
    ledc_channel_config(&ledc_channel);
}

static void init_hw2(void)
{
    adc2_config_channel_atten(adc2_channel, ADC_ATTEN_DB_11);
}

void app_main()
{
    init_hw1();
    init_hw2();
    double time = 0.0;
    while (1)
    {  
        time += 0.1;
        int adc_val = 0;
        esp_err_t err = adc2_get_raw(adc2_channel, ADC_WIDTH_BIT_10, &adc_val);
        ESP_LOGI("TAG", "%d - %d\n", err, adc_val);
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, adc_val);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}