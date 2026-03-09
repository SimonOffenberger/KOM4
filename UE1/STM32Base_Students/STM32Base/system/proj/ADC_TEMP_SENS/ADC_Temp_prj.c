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

#include "sysdelay.h"
#include "board_led.h"
#include "sysdelay.h"
#include "board_adc.h"
#include "ADC_Temp_prj.h"

// Temperature thresholds in degree Celsius
#define TEMP_LOW 20
#define TEMP_HIGH 27
#define LOWER_THREE_BITS_MASK 0x7

/**
 * @brief Main function for the Project ADC Temperature Sensor Exercise 8
 * 
 */
void main_ADC_Temp()
{

  // Initialize Peripheral
  LED_Initialize();
  ADC_Initialize(ADC_Channel_TempSensor);

  int8_t cnt = 0;
  while (1)
  {
    const int32_t temp = ADC_GET_int_Temperatur();
    uint32_t led_states = 0;

    // Update LED based on temperature if data is ready
    if (temp != ERROR_ADC_DATA_NOT_READY)
    {

      // Set LED6 color based on temperature thresholds
      if (temp < TEMP_LOW)
      {
        // Temperature below low threshold: Blue
        led_states |= LED6_BLUE_MASK;
      }
      else if (temp > TEMP_HIGH)
      {
        // Temperature above high threshold: Red and all other LEDs on
        led_states |= LED6_RED_MASK | LED3_MASK | LED4_MASK | LED5_MASK;
      }
      else
      {
        // Temperature within range: Green and indicate temperature on lower 3 LEDs
        led_states |= (temp - TEMP_LOW) & LOWER_THREE_BITS_MASK;
        led_states |= LED6_GREEN_MASK;
      }

      // Set the led Output
      LED_SetOut(led_states);
    }
  }
}