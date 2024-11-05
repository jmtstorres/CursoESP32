#include "wrp_wifi.h"

static EventGroupHandle_t s_wifi_event_group;

wrp_wifi_st status = WRP_WIFI_ST_DISCONNECTED;

bool initialized = false;

static void event_handler(
    void* arg, 
    esp_event_base_t event_base,
    int32_t event_id, 
    void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        ESP_LOGI(TAG, "Tentando conexão...");
        status = WRP_WIFI_ST_CONNECTING;
        int rc = esp_wifi_connect();
        ESP_LOGI(TAG, "Tentando conexão: %d", rc);
        while(rc != ESP_OK){
            rc = esp_wifi_connect();
            ESP_LOGI(TAG, "Tentando conexão: %d", rc);
            vTaskDelay(10000 / portTICK_PERIOD_MS);
        }
        status = WRP_WIFI_ST_CONNECTED;
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) {
        ESP_LOGI(TAG, "Conectado...");
        status = WRP_WIFI_ST_CONNECTED;
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        status = WRP_WIFI_ST_CONNECTING;
        ESP_LOGI(TAG, "Desconectado...");
        int rc = esp_wifi_connect();
        ESP_LOGI(TAG, "Tentando conexão: %d", rc);
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        status = WRP_WIFI_ST_CONNECTED;
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

int wifi_init(){
    ESP_LOGI(TAG, "Criando event handler");
    s_wifi_event_group = xEventGroupCreate();

    ESP_LOGI(TAG, "Iniciando rede");
    int rc = esp_netif_init();
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    ESP_LOGI(TAG, "Criando looper de eventos");
    rc = esp_event_loop_create_default();
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    if(!initialized){
        ESP_LOGI(TAG, "Iniciando configuração padrão de wifi");
        esp_netif_create_default_wifi_sta();
        initialized = true;
    }

    ESP_LOGI(TAG, "Iniciando Wifi...");
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    rc = esp_wifi_init(&cfg);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    rc = esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    rc = esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    return rc;
}

int wifi_connect(char *ssid, char *password){
    wifi_config_t wifi_config = {};
    strncpy( (char*)wifi_config.sta.ssid,ssid,32);
    strncpy( (char*)wifi_config.sta.password,password,64);

    ESP_LOGI(TAG, "Setting mode...");
    int rc = esp_wifi_set_mode(WIFI_MODE_STA);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    ESP_LOGI(TAG, "Setting config...");
    rc = esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }
    ESP_LOGI(TAG, "Starting...");
    rc = esp_wifi_start();
    if(rc != ESP_OK){
        ESP_LOGI(TAG, "Erro: %d", rc);
    }

    return 0;
}

int wifi_status(){
    return status;
}

void wifi_disconnect(){
    esp_wifi_disconnect();
    esp_wifi_stop();
}