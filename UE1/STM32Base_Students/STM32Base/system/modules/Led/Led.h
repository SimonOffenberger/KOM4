/**
 * @author Simon Offenberger
 * @version 0.1
 * @date 2025-10-25
 * 
 */
#ifndef LED_H
#define LED_H

#include "Gpio.h"

/**
 * @brief Function for initializing LEDs on the board
 * 
 * @return int Return value of GPIO_Initialize
 */
int LedInitialize();

/**
 * @brief Turn on a specified led
 * 
 * @param id ID of the GPIO from the LED
 */
void LedOn(const int id);

/**
 * @brief Turn off a specified led
 * 
 * @param id ID of the GPIO from the LED
 */
void LedOff(const int id);


#endif