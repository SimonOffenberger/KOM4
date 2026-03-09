/**
  ******************************************************************************
  * @file    3bit_cnt.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2017
  * @brief   APUS Board 3Bit CNT source File.
  ******************************************************************************
  */

#include "stm32f0xx_gpio.h"
#include "board_led.h"
#include "board_button.h"
#include "3bit_cnt.h"
#include "sysdelay.h"

/**
  * @brief  Implimentation of the 3bit counter which
	* @brief  displays the number on the LEDs
  * @param  None
  * @retval None
  */
void main_3bit_cnt(void){
  
  // Initialize the LEDS and Button
  LED_Initialize();
  Button_Initialize();

  while(1){
    // Counter variable
    static uint8_t cnt = 0;
  
    // collect the states of all the user Buttons
    const uint32_t button_States = Button_GetState();
   
    // if Button 1 is pressed count up 
    // if Button 0 is pressed cout down
    if     (button_States & BUTTON_USER0_MASK) cnt++;
    else if(button_States & BUTTON_USER1_MASK) cnt--;
    
    // limit the max value of the counter
    cnt &= MAX_CNT_VAL_MASK;
    
    // Write counter value to the leds
    LED_SetOut(cnt);
    
    // limit cycle time to see the counter counting
    SysDelay_Delay(250);
  }
}