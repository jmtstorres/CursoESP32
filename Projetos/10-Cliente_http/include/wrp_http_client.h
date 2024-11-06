#ifndef _WRP_HTTP_CLIENT
#define _WRP_HTTP_CLIENT

#include <wrp_utils.h>
#include "esp_http_client.h"
#include "esp_crt_bundle.h"

extern const uint8_t client_certificate_start[] asm("_binary_client_certificate_pem_start");
extern const uint8_t client_certificate_end[]   asm("_binary_client_certificate_pem_end");

extern const uint8_t client_key_start[] asm("_binary_client_key_pem_start");
extern const uint8_t client_key_end[]   asm("_binary_client_key_pem_end");

int wrp_http_post(const char *message, const char *url);
int wrp_http_get();
int wrp_http_close();

#endif