/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

poo

#include "Wireless.h"
#include "Gyro_QMI8658.h"
#include "RTC_PCF85063.h"
#include "SD_Card.h"
#include "LVGL_Driver.h"
#include "LVGL_Example.h"
#include "BAT_Driver.h"

#include "RadialSelector.h"

void Driver_Loop(void *parameter)
{
  while(1)
  {
    QMI8658_Loop();
    RTC_Loop();
    BAT_Get_Volts();
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
void Driver_Init()
{
  Flash_test();
  BAT_Init();
  I2C_Init();
  TCA9554PWR_Init(0x00);   
  Set_EXIO(EXIO_PIN8,Low);
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
void setup()
{
  //Wireless_Test2();
  Driver_Init();
  LCD_Init();                                     // If you later reinitialize the LCD, you must initialize the SD card again !!!!!!!!!!
  SD_Init();                                      // It must be initialized after the LCD, and if the LCD is reinitialized later, the SD also needs to be reinitialized
  Lvgl_Init();

  //lvgl_radial_selector();

  //Lvgl_Example1();
  //lv_demo_widgets();               
  // lv_demo_benchmark();          
  // lv_demo_keypad_encoder();     
  // lv_demo_music();              
  // lv_demo_printer();
  // lv_demo_stress();   
  lv_obj_clean(lv_scr_act()); // Clears all existing screen objects
  lvgl_radial_selector();  // must come before loop uses the pointer!
  Serial.printf("Screen: %d x %d\n", lv_disp_get_hor_res(NULL), lv_disp_get_ver_res(NULL));
}

float yaw = 0;

void loop() {
  static unsigned long lastUpdate = millis();
  unsigned long now = millis();
  float dt = (now - lastUpdate) / 1000.0f;
  lastUpdate = now;

  QMI8658_Loop();    // This calls getAccelerometer()
  getGyroscope();    // This updates Gyro.z
  yaw += Gyro.z * dt;

  update_selector_from_yaw(yaw);
  Lvgl_Loop();
  vTaskDelay(pdMS_TO_TICKS(5));
}

