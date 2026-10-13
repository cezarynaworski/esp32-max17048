#include "driver/i2c_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "include/max17048.h"
#include "sdkconfig.h"
#include <stdio.h>
#define I2C_MASTER_SCL_IO 17
#define I2C_MASTER_SDA_IO 16
#define I2C_MASTER_NUM I2C_NUM_0
#define I2C_MASTER_FREQ_HZ 400000
#define I2C_MASTER_TIMEOUT_MS 1000

static const char *TAG = "example";
static i2c_master_bus_handle_t bus_handle;

void app_main(void) {

  i2c_master_bus_config_t bus_config = {
      .i2c_port = I2C_MASTER_NUM,
      .sda_io_num = I2C_MASTER_SDA_IO,
      .scl_io_num = I2C_MASTER_SCL_IO,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .flags.enable_internal_pullup = false,
  };
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

  ESP_ERROR_CHECK(max17048_add_device_to_bus(&bus_handle));

  while (1) {
    float voltage = 0.0f;
    ESP_ERROR_CHECK(max17048_read_voltage(&voltage));
    ESP_LOGI(TAG, "Battery Voltage: %.3f mV", voltage);

    float soc = 0.0f;
    ESP_ERROR_CHECK(max17048_read_soc(&soc));
    ESP_LOGI(TAG, "State of Charge: %.2f %%", soc);

    uint16_t version = 0;
    ESP_ERROR_CHECK(max17048_read_version(&version));
    ESP_LOGI(TAG, "MAX17048 Version: 0x%04X", version);

    uint8_t device_id=0;
    ESP_ERROR_CHECK(max17048_read_sensor_id(&device_id));
    ESP_LOGI(TAG, "Device ID: 0x%02X", device_id);

    float soc_rate = 0;
    ESP_ERROR_CHECK(max17048_read_SOC_rate(&soc_rate));
    ESP_LOGI(TAG, "SOC Rate: %.5f %%/h", soc_rate);


    bool is_hibernating = false;
    ESP_ERROR_CHECK(max17048_read_hibernate_status(&is_hibernating));
    ESP_LOGI(TAG, "Is Hibernating: %s", is_hibernating ? "Yes" : "No");

    ESP_ERROR_CHECK(max17048_force_hibernate());
    ESP_LOGI(TAG, "Forced Hibernation Mode");

    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_ERROR_CHECK(max17048_read_hibernate_status(&is_hibernating));
    ESP_LOGI(TAG, "Is Hibernating: %s", is_hibernating ? "Yes" : "No");

    ESP_ERROR_CHECK(max17048_disable_hibernate());
    ESP_LOGI(TAG, "Disabled Hibernation Mode");

    vTaskDelay(pdMS_TO_TICKS(2000));

    ESP_ERROR_CHECK(max17048_read_hibernate_status(&is_hibernating));
    ESP_LOGI(TAG, "Is Hibernating: %s", is_hibernating ? "Yes" : "No");
  }
}
