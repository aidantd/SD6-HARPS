// Includes
#include "i2c.h"

// External Dependencies

// Declarations
#define DISABLE_MASTER_RX_BUFFER 0
#define DISABLE_MASTER_TX_BUFFER 0

#define DEV_BOARD

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