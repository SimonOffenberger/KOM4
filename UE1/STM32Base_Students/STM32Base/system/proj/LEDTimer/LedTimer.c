/**
  ******************************************************************************
  * @file    LedTimer.c
  * @author  Simon Offenberger
  * @version V1.0
  * @date    14-October-2017
  * @brief   APUS Board LED Timer source File.
  ******************************************************************************
  */
#include "board_led.h"
#include "board_button.h"
#include "LedTimer.h"
#include "sysdelay.h"
#include "rgb_timer.h"

/**
  * @brief  Implimentation of a blinking LED using a Timer
  * @param  None
  * @retval None
  */
void main_led_timer(void){
  
  // Initialize the LEDS, Button and Timer of the RGB LED
  LED_Initialize();
  Button_Initialize();
  RGB_LED_Initialize_PWM();

  // Intialize the Timer with 2Hz and 0% Duty Cycle
  // So the LED is off at the beginning
  RGB_Timer_Initialize(2 HZ,0 DutyCycle);
  
  while(1){
    // Read the Button State
    const uint32_t ButtonState = Button_GetState();
    static uint32_t lastButtonState = 0;

    // Change the Timer settings depending on the Button pressed
    if((ButtonState & BUTTON_USER0_MASK)  && lastButtonState == 0)  RGB_Timer_ChangeInterval(2 HZ ,16 DutyCycle);
    if((ButtonState & BUTTON_USER1_MASK)  && lastButtonState == 0)  RGB_Timer_ChangeInterval(15 HZ,25 DutyCycle);
    if((ButtonState & BUTTON_WAKEUP_MASK) && lastButtonState == 0)  RGB_Timer_ChangeInterval(9 HZ ,60 DutyCycle);

    // Store the last Button State for edge detection
    lastButtonState = ButtonState;

    // delay the loop to avoid bouncing effects of the buttons
    SysDelay_Delay(10);
  }
}