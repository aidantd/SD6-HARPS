// Includes
#include "uart.h"

// External Dependencies

// Declarations
#define SYSTEM_BAUD_RATE 115200
#define READ_WAIT_TIME 250

esp_err_t hal_master_init(void) {
    esp_err_t error = ESP_OK;
    const uart_port_t uart_num = UART_NUM_0;

    uart_config_t configuration = {
        .baud_rate = SYSTEM_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0};

    error |= uart_param_config(uart_num, &configuration);

    error |= uart_set_pin(uart_num, GPIO_NUM_1, GPIO_NUM_3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    error |= uart_driver_install(uart_num, 2048, 2048, 0, NULL, 0);

    return error;
}

esp_err_t uart_write_to_line(uint8_t *data, size_t size) {
    return uart_write_bytes(UART_NUM_0, (const char *)data, size);
}

esp_err_t uart_read_from_line(uint8_t *buffer, size_t size) {
    return uart_read_bytes(UART_NUM_0, buffer, size, READ_WAIT_TIME);
}
