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

    i2c_config_t configuration = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 10000,
        .clk_flags = 0,
    };

    i2c_param_config(i2c_master_port, &configuration);

    return i2c_driver_install(i2c_master_port, configuration.mode, DISABLE_MASTER_RX_BUFFER, DISABLE_MASTER_TX_BUFFER, 0);
}

esp_err_t i2c_write_to_device(uint8_t deviceAddress, uint8_t *data, uint8_t registerAddress, size_t size) {
    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    // Queue I2C commands to write to device
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(handle, registerAddress, true);
    i2c_master_write(handle, data, size, true);
    i2c_master_stop(handle);

    // Execute I2C commands
    esp_err_t error = i2c_master_cmd_begin(I2C_NUM_0, handle, 1000 / portTICK_PERIOD_MS);

    i2c_cmd_link_delete(handle);
    return error;
}

esp_err_t i2c_read_from_device(uint8_t deviceAddress, uint8_t *buffer, uint8_t registerAddress, size_t size) {
    i2c_cmd_handle_t handle = i2c_cmd_link_create();

    // Queue I2C commands to read from device
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(handle, registerAddress, true);
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (deviceAddress << 1) | I2C_MASTER_READ, true);
    i2c_master_read(handle, buffer, size, I2C_MASTER_LAST_NACK);
    i2c_master_stop(handle);

    // Execute I2C commands
    esp_err_t error = i2c_master_cmd_begin(I2C_NUM_0, handle, 1000 / portTICK_PERIOD_MS);

    i2c_cmd_link_delete(handle);
    return error;
}