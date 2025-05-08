#include "driver/uart.h"
#include "LVGL_Driver.h"
#include "SD_Card.h"
#include "BAT_Driver.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"

#define UART_PORT UART_NUM_0
#define UART_RX_PIN 44
#define UART_TX_PIN 43
#define BUF_SIZE 128

lv_obj_t *label;

void Driver_Loop(void *parameter) {
  while (1) {
    QMI8658_Loop();
    RTC_Loop();
    BAT_Get_Volts();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void Driver_Init() {
  Flash_test();
  BAT_Init();
  I2C_Init();
  TCA9554PWR_Init(0x00);
  Set_EXIO(EXIO_PIN8, Low);
  PCF85063_Init();
  QMI8658_Init();

  xTaskCreatePinnedToCore(
    Driver_Loop,
    "Other Driver task",
    4096,
    NULL,
    3,
    NULL,
    0
  );
}

void setup() {
  // USB CDC for debug
  Serial.begin(115200);
  printf("ESP32-S3 setup started\n");

  // Init hardware
  Driver_Init();
  LCD_Init();
  SD_Init();
  Lvgl_Init();

  // Clean default LVGL screen and show a label
  lv_obj_clean(lv_scr_act());
  label = lv_label_create(lv_scr_act());
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label, "Waiting for data...");

  // UART0 (RX=44, TX=43)
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity    = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
  };
  uart_param_config(UART_PORT, &uart_config);
  uart_set_pin(UART_PORT, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
  uart_driver_install(UART_PORT, BUF_SIZE * 2, 0, 0, NULL, 0);
}

void loop() {
  static uint32_t t = 0;

  // Print a heartbeat to CDC
  if (millis() - t > 1000) {
    printf("Time: %lu ms\n", millis());
    t = millis();
  }

  // Read incoming serial data
  uint8_t data[BUF_SIZE + 1];  // One extra for null terminator
  int len = uart_read_bytes(UART_PORT, data, BUF_SIZE, 10 / portTICK_PERIOD_MS);
  if (len > 0) {
    data[len] = '\0';  // Null-terminate
    printf("UART0: %s\n", (char *)data);
    lv_label_set_text(label, (char *)data);  // Update screen
  }

  Lvgl_Loop();  // Required for UI to update
  vTaskDelay(pdMS_TO_TICKS(5));
}