#ifndef FW_ERR_H
#define FW_ERR_H

#include "esp_err.h"

/* Project-wide error type; backed by ESP'IDF for now, swappable from this file only */
typedef esp_err_t fw_err_t; 

#define FW_OK                   ESP_OK
#define FW_FAIL                 ESP_FAIL
#define FW_ERR_INVALID_ARG      ESP_ERR_INVALID_ARG
#define FW_ERR_NOT_SUPPORTED    ESP_ERR_NOT_SUPPORTED
#define FW_ERR_INVALID_STATE    ESP_ERR_INVALID_STATE
#define FW_ERR_TIMEOUT          ESP_ERR_TIMEOUT

/* Stops with a message if 'x' is not FW_OK */
#define FW_ERR_CHECK(x)         ESP_ERROR_CHECK(x)

/* Readable name of an error code, for printf */
#define fw_err_to_name(code)    esp_err_to_name(code)

#endif 