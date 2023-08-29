// Includes
#include <stdio.h>

#include "driver/uart.h"
#include "esp_err.h"
#include "hal/gpio_types.h"

// External Dependencies

// Declarations

esp_err_t uart_master_init(void);
esp_err_t uart_write_to_line(uint8_t *data, size_t size);
esp_err_t uart_read_from_line(uint8_t *buffer, size_t size);