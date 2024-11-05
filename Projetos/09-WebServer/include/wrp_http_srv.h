#ifndef _WRP_HTTP_SRV
#define _WRP_HTTP_SRV

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include "esp_netif.h"
#include "esp_tls_crypto.h"
#include <esp_http_server.h>
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_tls.h"

httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);

#endif