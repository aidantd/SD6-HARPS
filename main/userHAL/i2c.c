// Includes
#include "i2c.h"

// External Dependencies

// Declarations
#define DISABLE_MASTER_RX_BUFFER 0
#define DISABLE_MASTER_TX_BUFFER 0

#define DEV_BOARD
#define I2C_BUFFER_SIZE 1024

esp_err_t i2c_master_init(void) {
    int i2c_master_port = I2C_NUM_0;

#ifdef DEV_BOARD
    i2c_config_t configuration = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};
#endif

#ifdef PCB_BOARD
    i2c_config_t configuration = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_2,
        .scl_io_num = GPIO_NUM_14,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000};
#endif

    i2c_param_config(i2c_master_port, &configuration);

    return i2c_driver_install(i2c_master_port, configuration.mode, DISABLE_MASTER_RX_BUFFER, DISABLE_MASTER_TX_BUFFER, 0);
}

esp_err_t i2c_write_to_device(uint8_t deviceAddress, uint8_t *data, size_t size) {
    uint8_t i2cBuffer[I2C_BUFFER_SIZE];
    i2c_cmd_handle_t handle = i2c_cmd_link_create_static(i2cBuffer, I2C_BUFFER_SIZE);

    if (i2c_master_start(handle) == ESP_OK) {
        if (i2c_master_write_byte(handle, (deviceAddress << 1) | I2C_MASTER_WRITE, true) == ESP_OK) {
            if (i2c_master_write(handle, data, size, true) == ESP_OK) {
                return i2c_master_stop(handle);
            }
        }
    }

    return ESP_FAIL;
}

esp_err_t i2c_read_from_device(uint8_t deviceAddress, uint8_t *buffer, size_t size) {
    uint8_t i2cBuffer[I2C_BUFFER_SIZE];
    i2c_cmd_handle_t handle = i2c_cmd_link_create_static(i2cBuffer, I2C_BUFFER_SIZE);

    if (i2c_master_start(handle) == ESP_OK) {
        if (i2c_master_write_byte(handle, (deviceAddress << 1) | I2C_MASTER_READ, true) == ESP_OK) {
            if (i2c_master_read(handle, buffer, size, I2C_MASTER_LAST_NACK) == ESP_OK) {
                return i2c_master_stop(handle);
            }
        }
    }

    return ESP_FAIL;
}