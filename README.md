# esp32-max17048

A simple abstraction layer for ESP-IDF I2C to control the MAX17048. It provides functions to read battery voltage, state of charge, chip version and status, and to control hibernation mode.

## Requirements

- ESP-IDF v5.3.5
- A MAX17048 connected to the I2C bus

No additional components are required.

## Usage

Add `main/src/max17048.c` and `main/include/max17048.h` to your project component, then configure the I2C bus and add the sensor:

```c
#include "driver/i2c_master.h"
#include "esp_err.h"
#include "include/max17048.h"

#define I2C_SDA_IO 16
#define I2C_SCL_IO 17

static i2c_master_bus_handle_t i2c_bus;

void app_main(void)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = I2C_SDA_IO,
        .scl_io_num = I2C_SCL_IO,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = false,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));
    max17048_add_device_to_bus(&i2c_bus);

    float voltage = 0.0f;
    float soc = 0.0f;
    ESP_ERROR_CHECK(max17048_read_voltage(&voltage));
    ESP_ERROR_CHECK(max17048_read_soc(&soc));
}
```

The default sensor address is `0x36`. The example uses GPIO 16 for SDA and GPIO 17 for SCL; change these pins to match your setup.

## Configuration

Pass a `max17048_config_t` to `max17048_configure()` to apply the sleep setting and hibernate, voltage, and SOC thresholds, along with the voltage-reset and SOC-change alert settings.

To read alert flags, pass a `max17048_status_t` to `max17048_read_status_register()`:


## Build

From the project directory, after setting up the ESP-IDF environment, run:

```sh
idf.py build
```
