/**
 * @file ADC_Temp_prj.c
 * @author Simon Offenberger
 * @brief ADC Temperature Sensor Project which indicates the internal Temperature on the LEDs
 * @version 1.0
 * @date 2025-12-06
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "board_led.h"
#include "sysdelay.h"
#include "i2c_temp.h"
#include "i2c.h"
#include "Lcd.h"


/**
 * @brief Main function for the Project I2C Temperature Sensor Exercise 8
 * 
 */
void main_I2C_Temp()
{

  // Initialize Peripheral
  LED_Initialize();
	Lcd_Initialize();
	Lcd_Fill(0,0,LCD_WIDTH,LCD_HEIGHT,RGB565_WHITE);
  I2C_Initialize(I2C_RTC_TEMP);

  while (1)
  {

		Lcd_Print(0,0,"Hello World",RGB565_BLACK,RGB565_WHITE,&Consolas);

    
    SysDelay_Delay(I2C_POLL_INTERVAL_MS);
  }
}