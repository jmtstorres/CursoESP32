//Adaptado de:
// SPIFFS filesystem usage example
//https://github.com/SIMS-IOT-Devices/FreeRTOS-ESP-IDF-SPIFFS/tree/main

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"

#define FILE_TEST "hello.txt"

static const char *TAG = "SPIFFS";

/*
    Deve ser criado e declarado um arquivo partitions.csv com as informacoes do disco a ser criado
    apos criado, deve ser declarado no platformio.ini (verificar o arquivo deste projeto)
    - em caso de falta de espaço, alterar a configuracao do compilador via menuconfig para otimizar
      tamanho do app
*/

void app_main(void)
{
    //Configurando a particao
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = true};
    //Registrando configuracao
    esp_vfs_spiffs_register(&conf);
    size_t total = 0, used = 0;
    //Recuperando informacoes do disco criado
    esp_spiffs_info(conf.partition_label, &total, &used);

    // Cria um arquivo e escreve dados
    char filename[312];
    memset(filename, 0x00, sizeof(filename));
    sprintf(filename, "/spiffs/%s", FILE_TEST);

    FILE *f = fopen(filename, "w");
    fprintf(f, "Hello ESP!\n");
    fclose(f);
    ESP_LOGI(TAG, "Criado.");
    // Abrindo arquivos no diretório para leitura
    ESP_LOGI(TAG, "Lendo arquivos");
    DIR *d;
    struct dirent *dir;
    d = opendir("/spiffs");
    if (d)
    {
        //Listando arquivos no diretório e lendo o conteúdo
        while ((dir = readdir(d)) != NULL)
        {
            ESP_LOGI(TAG, "%s", dir->d_name);
            memset(filename, 0x00, sizeof(filename));
            sprintf(filename, "/spiffs/%s", dir->d_name);
            f = fopen(filename, "r");
            char line[32];
            fgets(line, sizeof(line), f);
            fclose(f);
            char *pos = strchr(line, '\n');
            if (pos)
            {
                *pos = '\0';
            }
            ESP_LOGI(TAG, "Lido: '%s'", line);
        }
        closedir(d);
    }

    // Desmontando e desabilitando SPIFFS
    esp_vfs_spiffs_unregister(conf.partition_label);
    ESP_LOGI(TAG, "finalizado");
}