#ifndef _WRP_WIFI
#define _WRP_WIFI

#include "wrp_utils.h"
#include "esp_wifi.h"

#define wrp_wifi_st int

#define WRP_WIFI_ST_CONNECTED       0
#define WRP_WIFI_ST_CONNECTING      1
#define WRP_WIFI_ST_DISCONNECTED    2
#define WRP_WIFI_ST_ERROR           3
#define WRP_WIFI_ST_GOT_IP          4

int wifi_init();
int wifi_connect(char *ssid, char *password);
int wifi_status();
void wifi_disconnect();

#endif