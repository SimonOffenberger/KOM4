/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#include "Led.h"

/**
 * @brief Function for initializing LEDs on the board
 * 
 * @return int Return value of GPIO_Initialize
 */
int LedInitialize(){
  int retval = 0;
  
  retval |= Gpio_Initialize(GPIO_ID_LED3  ,GPIO_MODE_OUTPUT,true);
  retval |= Gpio_Initialize(GPIO_ID_LED4  ,GPIO_MODE_OUTPUT,true);
  retval |= Gpio_Initialize(GPIO_ID_LED5  ,GPIO_MODE_OUTPUT,true);

  return retval;
}

/**
 * @brief Turn on a specified led
 * 
 * @param id ID of the GPIO from the LED
 */
void LedOn(const int id){
  // leds are low active
	Gpio_Set(id,false);
}

/**
 * @brief Turn off a specified led
 * 
 * @param id ID of the GPIO from the LED
 */
void LedOff(const int id){
  // leds are low active
	Gpio_Set(id,true);
}
